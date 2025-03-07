/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/07 01:51:48 by lumartin         ###   ########.fr       */
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

int	num_redir_and_file(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] && (str[i] == '<' || str[i] == '>'))
		{
			count++;
			i++;
			if (str[i] == '>')
			{
				count++;
				i++;
			}
			while (ft_isspace(str[i]) && str[i])
			{
				count++;
				i++;
			}
			while (!ft_isspace(str[i]) && str[i])
			{
				count++;
				i++;
			}
		}
		i++;
	}
	return (count);
}

char	*clean_redirections(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*new_str;

	i = 0;
	j = 0;
	printf("str: %s\n", str);
	len = ft_strlen(str) - num_redir_and_file(str);
	printf("len: %d\n", len);
	new_str = malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	while (str[j])
	{
		if (str[j] && str[j + 1] && (str[j] == '<' || str[j] == '>'))
		{
			j++;
			if (str[j + 1] == '>')
				j++;
			while (ft_isspace(str[i]) && str[i])
				j++;
			while (!ft_isspace(str[i]) && str[i])
				j++;
		}
		j++;
		printf("str[%c]\n", str[j]);
		new_str[i] = str[j];
		i++;
		j++;
	}
	printf("clean str; %s\n", new_str);
	new_str[i] = '\0';
	return (new_str);
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

int	first_command(char **env, t_token *tokens, int num_commands, int *count)
{
	int		fds[2];
	int		connect[2];
	char	**args;
	int		i;
	pid_t	pid;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(tokens, &fds, num_commands, *count);
	args = build_command_string(tokens, num_commands, count);
	if (!args)
		return (ERROR);
	i = 0;
	while (args[i])
	{
		printf("full command [%d] %s\n", i, args[i]);
		i++;
	}
	if (num_commands == 1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (ERROR);
		}
		else if (pid == 0)
		{
			if (fds[0] != STDIN_FILENO)
			{
				dup2(fds[0], STDIN_FILENO);
				close(fds[0]);
			}
			if (fds[1] != STDOUT_FILENO)
			{
				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
			}
			exe(env, args);
			exit(EXIT_FAILURE);
		}
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return (STDOUT_FILENO);
	}
	else
	{
		if (pipe(connect) == -1)
		{
			perror("pipe");
			return (ERROR);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			close(connect[0]);
			close(connect[1]);
			return (ERROR);
		}
		else if (pid == 0)
		{
			close(connect[0]);
			dup2(connect[1], STDOUT_FILENO);
			close(connect[1]);
			if (fds[0] != STDIN_FILENO)
			{
				dup2(fds[0], STDIN_FILENO);
				close(fds[0]);
			}
			if (fds[1] != STDOUT_FILENO)
			{
				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
			}
			exe(env, args);
			exit(EXIT_FAILURE);
		}
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
		close(connect[1]);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return (connect[0]);
	}
}

int	pipex(char *argv_str, t_token *tokens)
{
	int		fd_in;
	int		pos[2];
	int		num_commands;
	int		i;
	char	**env;
	int		count;
	char	**cmd_array;
	int		status;
	pid_t	pid_wait;

	cmd_array = NULL;
	env = join_env(tokens->env_mshell);
	if (!env)
		return (ERROR);
	num_commands = num_pipes(argv_str) + 1;
	printf("num_commands: %d\n", num_commands);
	count = 0;
	fd_in = first_command(env, tokens, num_commands, &count);
	if (fd_in < 0)
	{
		free(env);
		return (ERROR);
	}
	if (num_commands == 1)
	{
		if (fd_in != STDOUT_FILENO)
			close(fd_in);
		pid_wait = waitpid(-1, &status, 0);
		if (pid_wait == -1)
			perror("waitpid");
		if (env)
		{
			i = 0;
			while (env[i])
				free(env[i++]);
			free(env);
		}
		return (0);
	}
	if (num_commands > 1)
	{
		cmd_array = ft_split(argv_str, '|');
		if (!cmd_array)
		{
			free(env);
			return (ERROR);
		}
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
		while (cmd_array[i])
			free(cmd_array[i++]);
		free(cmd_array);
		i = 0;
		while (i < num_commands)
		{
			pid_wait = waitpid(-1, &status, 0);
			if (pid_wait == -1)
				break ;
			i++;
		}
	}
	if (env)
	{
		i = 0;
		while (env[i])
			free(env[i++]);
		free(env);
	}
	return (0);
}
