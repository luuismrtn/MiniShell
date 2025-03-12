/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 16:39:12 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd(char **args, t_token **tokens)
{
	char	*path;

	if (args[1] && args[2])
	{
		exit_num = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	path = args[1];
	if (path == NULL)
	{
		path = search_path(join_env((*tokens)->env_mshell), "HOME")[1];
		if (path == NULL || chdir(path) != 0)
		{
			exit_num = 1;
			perror("minishell");
		}
	}
	else if (chdir(path) == 0)
		modify_pwd(tokens, "PWD", path);
	else
	{
		exit_num = 1;
		perror("minishell");
	}
}
