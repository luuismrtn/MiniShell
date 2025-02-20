/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/06 14:36:45 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	exe(char **env, char **comnd)
{
	char	**paths;
	int		i;

	if ((ft_strchr(comnd[0], '/') != NULL) && (access(comnd[0], X_OK) == 0))
		execve(comnd[0], comnd, env);
	else
	{
		paths = search_path(env, comnd[0]);
		i = 0;
		while (paths[i] != NULL)
		{
			if (access(paths[i], R_OK | X_OK) == 0)
				execve(paths[i], comnd, env);
			i++;
		}
		i = 0;
		while (paths[i])
			free(paths[i++]);
		free(paths);
	}
	perror("error command");
	exit(9);
}

int	first_command(char **argv, char **env)
{
	int		fd_in;
	char	**comnd1;
	int		connect[2];

	pipe(connect);
	if (fork() == 0)
	{
		close(connect[0]);
		dup2(connect[1], 1);
		close(connect[1]);
		fd_in = open(argv[1], O_RDONLY);
		if (fd_in < 0)
		{
			perror("error input file");
			exit(3);
		}
		comnd1 = ft_split(argv[2], ' ');
		if (!comnd1)
			exit(4);
		dup2(fd_in, 0);
		close(fd_in);
		exe(env, comnd1);
	}
	close(connect[1]);
	return (connect[0]);
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

void	final_command(char **argv, int *pos, char **env, int fd_in)
{
	int		fd_out;
	char	**comnd2;

	if (fork() == 0)
	{
		fd_out = open(argv[pos[OUTFILE]], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd_out < 0 || fd_in < 0)
		{
			perror("error output file");
			exit(7);
		}
		comnd2 = ft_split(argv[pos[END_COMD]], ' ');
		if (!comnd2)
			exit(8);
		dup2(fd_in, 0);
		close(fd_in);
		dup2(fd_out, 1);
		close(fd_out);
		exe(env, comnd2);
	}
	else
		close(fd_in);
}

int	main(int argc, char **argv, char **env)
{
	int	fd_in;
	int	i;
	int	pos[2];

	if (argc < 5 || ok_args(argc, argv) < (argc - 3))
	{
		write(1, "missing param\n", 14);
		exit(1);
	}
	fd_in = first_command(argv, env);
	i = 3;
	while (i < argc - 2)
	{
		fd_in = middle_command(argv, i, env, fd_in);
		i++;
	}
	pos[END_COMD] = i;
	pos[OUTFILE] = argc - 1;
	final_command(argv, pos, env, fd_in);
	while (argc - 3)
	{
		waitpid(-1, NULL, 0);
		argc--;
	}
}
