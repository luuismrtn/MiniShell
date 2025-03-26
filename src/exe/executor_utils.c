/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:00:46 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/26 19:42:41 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	errors_pipex(int *pipe_in, int *pipe_out, char **args, char c)
{
	free_array(args);
	if (c == 'p')
		perror("pipe");
	if (c == 'f')
	{
		perror("fork");
		close((*pipe_in));
		close((*pipe_out));
	}
	if (c == 'e') // fork final_command
	{
		perror("fork");
		close((*pipe_in));
	}
	if (c == 'd') // fd_in
	{
		perror("error output file");
		exit(1);
	}
}

int	child_pipe_fdin_redir(int *fd_in, char **args, int (*connect)[2]) //cambia pipe/fd_in entrane
{
	int	original_stdout;

	original_stdout = STDOUT_FILENO;
	if (fd_in != NULL)
	{
		if ((*fd_in) < 0)
			errors_pipex(NULL, NULL, args, 'd');
		dup2((*fd_in), STDIN_FILENO);
		close((*fd_in));
	}
	if (connect && ((*connect)[0] || (*connect)[1]))
	{
		close((*connect)[0]);
		original_stdout = dup(STDOUT_FILENO);
		dup2((*connect)[1], STDOUT_FILENO);
		close((*connect)[1]);
	}
	return (original_stdout);
}

//cambia fds-redirecciones antes d ejecutar
void	change_fds_redir(int (*fds)[2], int *o_stdin, int *o_stdout,
		int executor_father) 
{
	if ((*fds)[0] != STDIN_FILENO)
	{
		if (executor_father == 1)
			(*o_stdin) = dup(STDIN_FILENO);
		dup2((*fds)[0], STDIN_FILENO);
		close((*fds)[0]);
	}
	if ((*fds)[1] != STDOUT_FILENO)
	{
		if (executor_father == 1)
			(*o_stdout) = dup(STDOUT_FILENO);
		dup2((*fds)[1], STDOUT_FILENO);
		close((*fds)[1]);
	}
}

void	clean_father_material(int (*fds)[2], char **args)
{
	if ((*fds)[0] != STDIN_FILENO)
		close((*fds)[0]);
	if ((*fds)[1] != STDOUT_FILENO)
		close((*fds)[1]);
	free_array(args);
}
