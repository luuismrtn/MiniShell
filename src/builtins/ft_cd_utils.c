/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:49:21 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/21 15:12:04 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Cuenta los niveles de directorio y acumula los niveles a subir.
 *
 * Recorre un array de componentes de ruta y cuenta cuántos niveles de
 * directorio
 * existen. Además, incrementa el contador de niveles a subir por cada ".."
 * encontrado.
 *
 * @param components Array de strings con componentes de una ruta.
 * @param levels_up Puntero a un contador de niveles a subir,
 * aumenta con cada "..".
 * @return int Número total de componentes en la ruta.
 */
static int	count_directory_levels(char **components, int *levels_up)
{
	int	i;

	i = 0;
	while (components && components[i])
	{
		if (ft_strncmp(components[i], "..", 3) == 0)
			(*levels_up)++;
		i++;
	}
	return (i);
}

/**
 * @brief Calcula la ruta deseada después de aplicar cambios relativos.
 *
 * Esta función toma una ruta actual (pwd) y una ruta relativa (dir),
 * y calcula la ruta absoluta resultante después de aplicar la ruta relativa.
 * Es especialmente útil cuando getcwd() falla y necesitamos calcular la ruta
 * programáticamente.
 *
 * @param pwd La ruta actual (directorio de trabajo).
 * @param dir La ruta relativa a aplicar.
 * @return char* La nueva ruta absoluta (debe ser liberada por el llamador).
 */
static char	*find_desired_path(char *pwd, char *dir)
{
	char	**components_dir;
	char	**components_pwd;
	char	*result;
	int		levels_up;
	int		i;

	levels_up = 0;
	components_dir = ft_split(dir, '/');
	components_pwd = ft_split(pwd, '/');
	i = count_directory_levels(components_pwd, &levels_up);
	levels_up *= 2;
	i = count_directory_levels(components_dir, &levels_up);
	free_array(components_dir);
	free_array(components_pwd);
	i = ft_strlen(pwd);
	while (i > 0 && levels_up > 0)
	{
		if (pwd[i] == '/')
			levels_up--;
		i--;
	}
	result = ft_substr(pwd, 0, i + 1);
	return (result);
}

/**
 * @brief Muestra un mensaje de error para el comando cd.
 *
 * Imprime un mensaje de error estandarizado para el comando cd cuando
 * una ruta no es válida o accesible. También establece el código de salida
 * en 1 para indicar que ocurrió un error.
 *
 * @param path La ruta que causó el error.
 */
void	print_cd_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit_num = 1;
}

/**
 * @brief Maneja el caso donde getcwd() falla pero necesitamos cambiar de
 * directorio.
 *
 * En algunos sistemas,	cuando el directorio de trabajo actual ya no es
 * accesible, getcwd() puede fallar. Esta función permite cambiar de
 * directorio incluso en esos casos calculando la ruta manualmente.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param input_path La ruta a la que queremos cambiar.
 */
void	handle_broken_pwd(t_token **tokens, char *input_path)
{
	char	*desired_path;
				t_env	*current;

	if (!validate_input_cd(input_path))
	{
		print_cd_error(input_path);
		return ;
	}
	desired_path = find_desired_path(find_env_var((*tokens)->env_mshell,
				"PWD")->content, input_path);
	if (chdir(desired_path) == 0)
	{
		get_env_content_and_replace(tokens, "PWD", desired_path);

		current = (*tokens)->env_mshell;
		while (current)
		{
			if (ft_strncmp(current->name, "PWD", ft_strlen("PWD")) == 0)
			{
				printf("content PWD: %s\n", current->content);
				break ;
			}
			current = current->next;
		}
	}
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		modify_pwd(tokens, input_path);
	}
	//free(desired_path); esto daba problema cd encuentra el dir q existe
}

/**
 * @brief Procesa la ruta proporcionada como argumento.
 *
 * Normaliza la ruta eliminando cualquier barra al final si existe.
 * Esto es útil para manejar rutas como "/home/" que deben tratarse
 * igual que "/home".
 *
 * @param args Array de argumentos del comando.
 * @return char* Ruta normalizada (debe ser liberada por el llamador).
 */
char	*find_path(char **args)
{
	char	*path;

	if (args[1][ft_strlen(args[1]) - 1] == '/')
		path = ft_substr(args[1], 0, ft_strlen(args[1]) - 1);
	else
		path = ft_strdup(args[1]);
	return (path);
}
