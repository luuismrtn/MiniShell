/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 21:29:29 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd(char **args, t_token **tokens)
{
	char	*path;

	if (!args[1])
	{
		path = search_path(join_env((*tokens)->env_mshell), "HOME")[1];
		chdir(get_env_content((*tokens)->env_mshell, "HOME"));
		modify_pwd(tokens, "PWD", "HOME");
		return ;
	}
	if (args[1] && args[2])
	{
		exit_num = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (args[1][ft_strlen(args[1]) - 1] == '/')
		path = ft_substr(args[1], 0, ft_strlen(args[1]) - 1);
	else
		path = ft_strdup(args[1]);
	if (getcwd(NULL, 0) == NULL)
	{
		printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
		modify_pwd(tokens, "PWD", path);
	}
	else
	{
		printf("ENCUENETRA CWD\n");
		if (chdir(path) == 0)
			modify_pwd(tokens, "PWD", path);
		else
		{
			exit_num = 1;
			ft_putstr_fd("cd: no such file or directory: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd("\n", 2);
		}
	}
}
