/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 22:20:23 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	cd_to_home(t_token **tokens)
{
	char	*home_path;

	home_path = get_env_content((*tokens)->env_mshell, "HOME");
	if (!home_path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return ;
	}
	if (chdir(home_path) == 0)
	{
		modify_pwd(tokens, "PWD", home_path);
	}
	else
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(home_path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

static char	*find_path(char **args)
{
	char	*path;

	if (args[1][ft_strlen(args[1]) - 1] == '/')
		path = ft_substr(args[1], 0, ft_strlen(args[1]) - 1);
	else
		path = ft_strdup(args[1]);
	return (path);
}

static void	execute_cd(char *path, t_token **tokens)
{
	if (getcwd(NULL, 0) == NULL)
	{
		printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
		modify_pwd(tokens, "PWD", path);
	}
	else
	{
		if (chdir(path) == 0)
			modify_pwd(tokens, "PWD", path);
		else
		{
			exit_num = 1;
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}
}

void	ft_cd(char **args, t_token **tokens)
{
	char	*path;

	if (!args[1])
		cd_to_home(tokens);
	else if (args[1] && args[2])
	{
		exit_num = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	else
	{
		path = find_path(args);
		execute_cd(path, tokens);
	}
}
