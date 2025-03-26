/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:29 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/26 19:36:49 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	executor_father(t_token *tokens, int (*fds)[2], char ***args,
		int original_stdout)
{
	int	original_stdin;

	original_stdin = -1;
	change_fds_redir(fds, &original_stdin, &original_stdout, 1);
	handle_builtin((*args), tokens);
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	if (original_stdout != -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
	free_array((*args));
}

void one_comnd(t_token **tokens)
{
	int		fds[2];
	char	**args;
	pid_t	pid;
	int		status;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(*tokens, &fds, 0);
	args = build_command_string(*tokens, 0);
	if (!args || !args[0])
		return ;
	if (is_builtin(args) == 1)
		return (executor_father((*tokens), &fds, &args, -1));
	signals('c');
	if (ft_strncmp(args[0], "./minishell", 12) == 0)
		ign_signal();
	pid = fork();
	if (pid == -1)
		return (perror("fork"));
	else if (pid == 0)
		executor(tokens, &fds, args, dup(STDOUT_FILENO));
	clean_father_material(&fds, args);
	waitpid(pid, &status, 0);
	exit_num = WEXITSTATUS(status);
	signals('f');
}
