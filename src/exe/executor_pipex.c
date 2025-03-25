/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/25 18:13:03 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	executor(t_token **tokens, int (*fds)[2], char **args,
		int original_stdout)
{
	if ((ft_strncmp(args[0], "./minishell", 12) == 0) && !has_pipe(*tokens))
		modify_shlvl(tokens, "SHLVL");
	if (is_builtin(args) == 1)
	{
		change_fds_redir(fds, NULL, &original_stdout, 1);
		handle_builtin(args, (*tokens));
	}
	else
	{
		change_fds_redir(fds, NULL, NULL, 0);
		exe((*tokens), args, original_stdout);
	}
	exit(exit_num);
}

int	middle_command(int *count, t_token **tokens, int fd_in)
{
	int		fds[2];
	int		connect[2];
	char	**args;
	pid_t	pid;
	int		original_stdout;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections((*tokens), &fds, *count);
	args = build_command_string((*tokens), *count);
	if (!args || !args[0])
		return (ERROR);
	if (pipe(connect) == -1)
		return (errors_pipex(NULL, NULL, &args, 'p'), ERROR);
	pid = fork();
	if (pid == -1)
		return (errors_pipex(&connect[0], &connect[1], &args, 'f'), ERROR);
	else if (pid == 0)
	{
		original_stdout = child_pipe_fdin_redir(&fd_in, args, &connect);
		executor(tokens, &fds, args, original_stdout);
	}
	clean_father_material(&fds, &args);
	return (close(fd_in), close(connect[1]), connect[0]);
}

void	final_command(int *count, t_token **tokens, int fd_in)
{
	int		fds[2];
	char	**args;
	pid_t	pid;
	int		status;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections((*tokens), &fds, *count);
	args = build_command_string((*tokens), *count);
	if (!args || !args[0])
		return ;
	signals('c');
	if (ft_strncmp(args[0], "./minishell", 12) == 0)
		ign_signal();
	pid = fork();
	if (pid == -1)
		return (errors_pipex(&fd_in, NULL, &args, 'e'));
	else if (pid == 0)
	{
		child_pipe_fdin_redir(&fd_in, args, NULL);
		executor(tokens, &fds, args, dup(STDOUT_FILENO));
	}
	waitpid(pid, &status, 0);
	exit_num = WEXITSTATUS(status);
	return (close(fd_in), free_array(args)); //signals('f')->a pipex
}

int	first_command(t_token **tokens, int count)
{
	int		fds[2];
	int		connect[2];
	char	**args;
	pid_t	pid;
	int		original_stdout;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(*tokens, &fds, count);
	args = build_command_string(*tokens, count);
	if (!args || !args[0])
		return (ERROR);
	if (pipe(connect) == -1)
		return (errors_pipex(NULL, NULL, &args, 'p'), ERROR);
	if (ft_strncmp(args[0], "./minishell", 12) == 0)
		return (close(connect[1]), connect[0]);
	pid = fork();
	if (pid == -1)
		return (errors_pipex(&connect[0], &connect[1], &args, 'f'), ERROR);
	else if (pid == 0)
	{
		original_stdout = child_pipe_fdin_redir(NULL, args, &connect);
		executor(tokens, &fds, args, original_stdout);
	}
	return (close(connect[1]), clean_father_material(&fds, &args), connect[0]);
}

void	pipex(t_token *tokens, int num_commands)
{
	int	fd_in;
	int	count;
	int	status;

	fd_in = first_command(&tokens, 0);
	if (num_commands > 1)
	{
		count = 1;
		while (count < num_commands - 1)
		{
			fd_in = middle_command(&count, &tokens, fd_in);
			count++;
		}
		if (count < num_commands)
			final_command(&count, &tokens, fd_in);
		count = 0;
		while (count < num_commands)
		{
			waitpid(-1, &status, 0);
			count++;
		}
		signals('f'); //señalado x si aca
	}
}
