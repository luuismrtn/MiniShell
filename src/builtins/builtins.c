/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:21:08 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/20 12:27:12 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_builtin(char **args, t_token *tokens)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		ft_echo(args);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		ft_cd(args, &tokens);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		ft_pwd(tokens);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		ft_export(tokens, args);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		ft_unset(tokens, args);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		ft_env(tokens->env_mshell);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		ft_exit(args);
	else
		return ;
}
