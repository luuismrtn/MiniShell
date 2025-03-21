/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/21 14:47:32 by aldferna         ###   ########.fr       */
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
	char	*oldpwd_path;

	oldpwd_path = find_env_var((*tokens)->env_mshell, "OLDPWD")->content;
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
	free(oldpwd_path);
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

	home_path = find_env_var((*tokens)->env_mshell, "HOME")->content;
	if (!home_path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		exit_num = 1;
		return ;
	}
	actual_path = find_env_var((*tokens)->env_mshell, "PWD")->content;
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
	char	*path;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0 || ft_strncmp(args[1],
			"--", 3) == 0)
		cd_to_home(tokens);
	else if (match_string(args[1], "/") || ft_strncmp(args[1], "//", 2) == 0)
		cd_to_root(tokens, args[1]);
	else if (match_string(args[1], "-") == 1)
		cd_to_oldpwd(tokens);
	else if (args[1] && args[2])
	{
		exit_num = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
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
