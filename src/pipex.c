/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/05 22:42:08 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/pipex.h"

static int	ok_args(int argc, char **arg)
{
	int	i;
	int	x;
	int	count;

	count = 0;
	i = 2;
	while (i < argc - 1)
	{
		x = 0;
		while (arg[i][x] != '\0')
		{
			if (arg[i][x] != ' ' && arg[i][x] != '\t' && arg[i][x] != '\n')
			{
				count++;
				break ;
			}
			x++;
		}
		i++;
	}
	return (count);
}

void	final_command(char **cmd_array, int *pos, char **env, int fd_in)
{
	int		fd_out;
	char	**comnd2;

	if (fork() == 0)
	{
		fd_out = STDOUT_FILENO;
		if (fd_in < 0)
		{
			perror("error output file");
			exit(7);
		}
		comnd2 = ft_split(cmd_array[pos[END_COMD]], ' ');
		if (!comnd2)
			exit(8);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		exe(env, comnd2);
		exit(EXIT_FAILURE);
	}
	else
		close(fd_in);
}

int	num_pipes(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count++;
		i++;
	}
	return (count);
}

int	middle_command(char **argv, int i, char **env, int fd_in)
{
	char	**comnd1;
	int		connect[2];

	pipe(connect);
	if (fork() == 0)
	{
		close(connect[0]);
		if (fd_in < 0)
		{
			perror("error input");
			exit(3);
		}
		comnd1 = ft_split(argv[i], ' ');
		if (!comnd1)
			exit(4);
		dup2(fd_in, 0);
		close(fd_in);
		dup2(connect[1], 1);
		close(connect[1]);
		exe(env, comnd1);
	}
	close(connect[1]);
	close(fd_in);
	return (connect[0]);
}

int	first_command(t_token *tokens, int num_commands, int *count, char **env)
{
	int		fds[2];
	int		connect[2];
	char	*args;
	char	**full_command;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(tokens, &fds, num_commands, count);
	args = build_command_string(tokens, 1, count);
	if (!args)
		return (ERROR);
	full_command = ft_split(args, ' ');
	if (!full_command)
		return (free(args), ERROR);
	pipe(connect);
	if (fork() == 0)
	{
		close(connect[0]);
		dup2(connect[1], 1);
		close(connect[1]);
		dup2(fds[0], 0);
		close(fds[0]);
		exe(env, full_command);
		exit(EXIT_FAILURE);
	}
	free(args);
	close(connect[1]);
	return (connect[0]);
}

int	pipex(char *argv_str, t_token *tokens, char **env)
{
	int		fd_in;
	int		pos[2];
	int		num_commands;
	int		count;
	int		i;
	char	**cmd_array;

	cmd_array = ft_split(argv_str, '|');
	if (!cmd_array)
		return (ERROR);
	num_commands = num_pipes(argv_str) + 1;
	count = 0;
	fd_in = first_command(tokens, num_commands, &count, env);
	i = 1;
	while (i < num_commands - 1)
	{
		if (cmd_array[i] != NULL)
			fd_in = middle_command(cmd_array, i, env, fd_in);
		else
			break ;
		i++;
	}
	if (i < num_commands && cmd_array[i] != NULL)
	{
		pos[END_COMD] = i;
		pos[OUTFILE] = i;
		final_command(cmd_array, pos, env, fd_in);
	}
	i = 0;
	while (i < num_commands)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
	i = 0;
	while (cmd_array[i])
	{
		free(cmd_array[i]);
		i++;
	}
	free(cmd_array);
	return (0);
}
