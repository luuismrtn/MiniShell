/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/19 19:18:03 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Cambia al directorio OLDPWD del usuario.
 *
 * Intenta cambiar al directorio oldpwd definido en la variable de entorno OLDPWD.
 * Si OLDPWD no está definido o no se puede acceder al directorio, muestra un
 * mensaje de error y establece el código de salida adecuado.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 */
static void	cd_to_oldpwd(t_token **tokens)
{
	char	*oldpwd_path;

	oldpwd_path = get_env_content((*tokens)->env_mshell, "OLDPWD");
	if (!oldpwd_path)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		exit_num = 1;
		return ;
	}
	if (chdir(oldpwd_path) == 0)
	{
		modify_pwd(tokens, oldpwd_path);
		exit_num = 0;
	}
	else
		print_cd_error(oldpwd_path);
}

/**
 * @brief Cambia al directorio ROOT del usuario.
 *
 * Cambia al directorio raíz "/" del sistema de archivos. Si no se puede
 * acceder al directorio, muestra un mensaje de error y establece el código
 * de salida adecuado.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param args Argumentos del comando (args[0] es "cd").
 */
static void	cd_to_root(t_token **tokens, char *args)
{
	if (ft_strncmp(args, "///", 3) == 0)
		args = ft_strdup("/");
	if (chdir(args) == 0)
	{
		modify_pwd(tokens, args);
		exit_num = 0;
	}
	else
		print_cd_error(args);
}

/**
 * @brief Cambia al directorio HOME del usuario.
 *
 * Intenta cambiar al directorio home definido en la variable de entorno HOME.
 * Si HOME no está definido o no se puede acceder al directorio, muestra un
 * mensaje de error y establece el código de salida adecuado.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 */
static void	cd_to_home(t_token **tokens)
{
	char	*home_path;
	char	*actual_path;

	home_path = get_env_content((*tokens)->env_mshell, "HOME");
	if (!home_path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		exit_num = 1;
		return ;
	}
	actual_path = get_env_content((*tokens)->env_mshell, "PWD");
	if (ft_strncmp(home_path, actual_path, ft_strlen(actual_path)))
	{
		if (chdir(home_path) == 0)
		{
			modify_pwd(tokens, home_path);
			exit_num = 0;
		}
		else
			print_cd_error(home_path);
	}
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
static char	*find_path(char **args)
{
	char	*path;

	if (args[1][ft_strlen(args[1]) - 1] == '/')
		path = ft_substr(args[1], 0, ft_strlen(args[1]) - 1);
	else
		path = ft_strdup(args[1]);
	return (path);
}

/**
 * @brief Valida si una ruta contiene solo referencias a directorios superiores.
 *
 * Comprueba si la ruta consiste únicamente en componentes ".." (directorios
 * padre).
 *
 * @param input La ruta a validar.
 * @return int 1 si la ruta solo contiene "..", 0 en caso contrario.
 */
static int	validate_input(char *input)
{
	char	**chop_input;
	int		i;
	int		ok;

	i = 0;
	ok = 0;
	chop_input = ft_split(input, '/');
	while (chop_input[i])
	{
		if (ft_strncmp(chop_input[i], "..", 3) == 0)
			ok++;
		i++;
	}
	free_array(chop_input);
	if (ok == i)
		return (1);
	return (0);
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
static void	handle_broken_pwd(t_token **tokens, char *input_path)
{
	char	*desired_path;

	if (!validate_input(input_path))
	{
		print_cd_error(input_path);
		return ;
	}
	desired_path = find_desired_path(get_env_content((*tokens)->env_mshell,
				"PWD"), input_path);
	if (chdir(desired_path) == 0)
		get_env_content_and_replace(tokens, "PWD", desired_path);
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		modify_pwd(tokens, input_path);
	}
	free(desired_path);
}

/**
 * @brief Implementa el comando cd (change directory).
 *
 * Cambia el directorio de trabajo actual al especificado, o al directorio
 * home si no se proporciona ningún argumento. Actualiza la variable de
 * entorno PWD después de un cambio exitoso.
 *
 * @param args Argumentos del comando (args[0] es "cd").
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 */
void	ft_cd(char **args, t_token **tokens)
{
	char	*path;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0 || ft_strncmp(args[1], "--", 3) == 0 )
		cd_to_home(tokens);
	else if (match_string(args[1], "/") == 1 || ft_strncmp(args[1], "//", 2) == 0)
		cd_to_root(tokens, args[1]);
	else if (match_string(args[1], "-") == 1)
		cd_to_oldpwd(tokens);
	else if (args[1] && args[2])
	{
		exit_num = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	else
	{
		path = find_path(args);
		if (getcwd(NULL, 0) == NULL)
			handle_broken_pwd(tokens, path);
		else if (chdir(path) == 0)
			modify_pwd(tokens, path);
		else
			print_cd_error(path);
		free(path);
	}
}
