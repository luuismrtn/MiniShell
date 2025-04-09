/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/09 16:47:14 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Cambia al directorio OLDPWD del usuario.
 *
 * Intenta cambiar al directorio oldpwd definido en la variable de entorno
 * OLDPWD.
 * Si OLDPWD no está definido o no se puede acceder al directorio, muestra un
 * mensaje de error y establece el código de salida adecuado.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 */
static void	cd_to_oldpwd(t_token **tokens)
{
	t_env	*oldpwd;

	oldpwd = find_env_var((*tokens)->env_mshell, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		g_exit_num = 1;
		return ;
	}
	if (chdir(oldpwd->content) == 0)
	{
		modify_pwd(tokens, oldpwd->content);
		printf("%s\n", oldpwd->content);
		g_exit_num = 0;
	}
	else
		print_cd_error(oldpwd->content, NULL, NULL);
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
	else
		args = ft_strdup(args);
	if (chdir(args) == 0)
	{
		modify_pwd(tokens, args);
		g_exit_num = 0;
	}
	else
		print_cd_error(args, NULL, NULL);
	free(args);
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
	t_env	*home_path_env;
	char	*home_path;
	char	*actual_path;

	home_path_env = find_env_var((*tokens)->env_mshell, "HOME");
	if (!home_path_env)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_exit_num = 1;
		return ;
	}
	home_path = home_path_env->content;
	actual_path = find_env_var((*tokens)->env_mshell, "PWD")->content;
	if (!match_string(home_path, actual_path))
	{
		if (chdir(home_path) == 0)
		{
			modify_pwd(tokens, home_path);
			g_exit_num = 0;
		}
		else
			print_cd_error(home_path, NULL, NULL);
	}
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
int	validate_input_cd(char *input)
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
	if (!args[1] || ft_strncmp(args[1], "--", 3) == 0)
		return (cd_to_home(tokens));
	else if (match_string(args[1], "/") || ft_strncmp(args[1], "//", 2) == 0)
		return (cd_to_root(tokens, args[1]));
	else if (match_string(args[1], "-") == 1)
		return (cd_to_oldpwd(tokens));
	else if (args[1] && args[2])
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	else
		handle_cd(tokens, args);
	g_exit_num = 1;
}
