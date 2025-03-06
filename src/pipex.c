/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/06 13:50:37 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/pipex.h"

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

int num_redir(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while(str[i])
	{
		if (str[i] && str[i + 1] && (str[i] == '<' || str[i] == '>'))
		{
			count++;
			if (str[i + 1] == '>')
				i++;
		}
		i++;
	}
	return count;
}

char *clean_redirections(char *str)
{
	int i;
	int j;
	int len;
	char *new_str;

	i = 0;
	j = 0;
	len = ft_strlen(str) - num_redir(str);
	new_str = malloc((len + 1) * sizeof(char));
	if (!new_str)
		return NULL;
	while (str[j])
	{
		if (str[j] && str[j + 1] && (str[j] == '<' || str[j] == '>'))
		{
			j++;
			if (str[j + 1] == '>')
				j++;
		}	
		new_str[i++] = str[j++];
	}
	new_str[i] = '\0';
	return (new_str);
}

int	first_command(char **argv, char **env, t_token *tokens, int num_commands)
{
	int		fds[2];
	int		connect[2];
	char	*clean_comnd;
	char	**full_command;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(tokens, &fds, num_commands, 0);
	clean_comnd = clean_redirections(argv[0]);
	//args = build_command_string(tokens, 1, count);
	if (!clean_comnd)
		return (ERROR);
	full_command = ft_split(clean_comnd, ' ');
	if (!full_command)
		return (free(clean_comnd), ERROR);
	int i = 0;
	while (full_command[i] != NULL)
	{
		printf("full command [%d]  %s\n", i, full_command[i]);
		i++;
	}
	free(clean_comnd);
	pipe(connect);
	if (fork() == 0)
	{
		close(connect[0]);
		dup2(connect[1], 1);
		close(connect[1]);
		if (fds[1] != STDOUT_FILENO)
		{
			dup2(fds[1], STDOUT_FILENO);
			close(fds[1]);		
		}
		if (fds[0] != STDIN_FILENO)
		{
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);		
		}
		exe(env, full_command);
		exit(EXIT_FAILURE);
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

void	final_command(char **cmd_array, int *pos, char **env, int fd_in)
{
	char	**comnd2;

	if (fork() == 0)
	{
		if (fd_in < 0)
		{
			perror("error output file");
			exit(7);
		}
		printf("cmd_array[pos[END_COMD]] = %s\n", cmd_array[pos[END_COMD]]);
		comnd2 = ft_split(ft_strtrim(cmd_array[pos[END_COMD]], "\n"), ' ');
		if (!comnd2)
			exit(1);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		exe(env, comnd2);
		exit(EXIT_FAILURE);
	}
	else
		close(fd_in);
}

int	pipex(char *argv_str, t_token *tokens)
{
	int		fd_in;
	int		pos[2];
	int		num_commands;
	int		i;
	char	**cmd_array;
	char	**env;

	env = join_env(tokens->env_mshell);
	cmd_array = ft_split(argv_str, '|');
	if (!cmd_array)
		return (ERROR);
	num_commands = num_pipes(argv_str) + 1;
	fd_in = first_command(cmd_array, env, tokens, num_commands);
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
