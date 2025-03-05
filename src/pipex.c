/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/05 20:31:41 by aldferna         ###   ########.fr       */
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

int num_pipes(char *str)
{
	int i;
	int count;

	while(str[i])
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

int	first_command(t_token *tokens, int num_commands, int *count)
{
	int		fds[2];
	char	**comnd1;
	int		connect[2];
	char	*args;
	char 	**full_command;
	t_env *env;
	
	env = tokens->env_mshell;
	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(tokens, &fds, num_commands, count);
	args = build_command_string(tokens, num_commands, count); //meter con comillas (modificar token)-> t_word pero contenido con comillas para no modif automata
	if (!args)
		return ERROR;
	full_command = ft_split(args, ' '); //y modificar el split para q salte la comilla+los espacios de dentro??
	if (!full_command)
		return (free(args), ERROR);
	//si solo 1 builtin-exit no hijo? cómo se gestiona el exit con pipe?
	pipe(connect);
	if (fork() == 0)
	{
		close(connect[0]);
		dup2(connect[1], 1);
		close(connect[1]);
		// fd_in = open(argv[1], O_RDONLY);
		// if (fd_in < 0)
		// {
		// 	perror("error input file");
		// 	exit(3);
		// }
		// comnd1 = ft_split(argv[2], ' ');
		// if (!comnd1)
		// 	exit(4);
		dup2(fds[0], 0);
		close(fds[0]);
		exe(env, comnd1); //modificar como tratar env para lista
	}
	close(connect[1]);
	return (connect[0]);
}

int	pipex(char *argv, t_token *tokens)
{
	int	fd_in;
	int	i;
	int	pos[2];
	int num_commands;
	int count;

	num_commands = num_pipes(argv) + 1;
	// if (argc < 5 || ok_args(argc, argv) < (argc - 3))
	// {
	// 	write(1, "missing param\n", 14);
	// 	exit(1);
	// }
	count = 0;
	fd_in = first_command(tokens, num_commands, &count);
	i = 3;
	while (i < argc - 2) //argc numero de pipes + 1??
	{
		fd_in = middle_command(tokens, fd_in, num_commands, &count);
		i++;
	}
	// pos[END_COMD] = i;
	// pos[OUTFILE] = argc - 1;
	final_command(tokens, fd_in, num_commands, &count);
	while (argc - 3)
	{
		waitpid(-1, NULL, 0);
		argc--;
	}
	return (0);
}
