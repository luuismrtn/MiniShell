/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/13 21:34:52 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void modify_shlvl(t_token **tokens, char *var)
{
	t_env *aux;
	t_env *new_var;
	int shlvl;

	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (strncmp(aux->name, var, strlen(var)) == 0)
		{
			shlvl = ft_atoi(aux->content) + 1;
			free(aux->content);
			aux->content = ft_itoa(shlvl);
			return;
		}
		aux = aux->next;
	}
	shlvl = 1;
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return;
	new_var->name = ft_strdup(var);
	new_var->content = ft_itoa(shlvl);
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}

int	middle_command(int *count, t_token **tokens, int fd_in)
{
	int 	fds[2];
	int		connect[2];
	char 	**args;
	pid_t	pid;
	int original_stdout;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections((*tokens), &fds, *count);
	args = build_command_string((*tokens), count);
	if (!args || !args[0])
		return (ERROR);

	if (pipe(connect) == -1)
	{
		perror("pipe");
		free_array(args);
		return (ERROR);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(connect[0]);
		close(connect[1]);
		free_array(args);
		return (ERROR);
	}
	else if (pid == 0)
	{
		close(connect[0]);
		if (fd_in >= 0)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		else
		{
			perror("fd_in\n");
			exit(1);
		}
		dup2(connect[1], STDOUT_FILENO);
		close(connect[1]);
		original_stdout = dup(STDOUT_FILENO);
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
		if (is_builtin(args) == 1)
		{
			handle_builtin(args, (*tokens));
			exit (0);
		}
		else
		{
			exe((*tokens), args, original_stdout);
			exit(EXIT_FAILURE);
		}
	}
	free_array(args);
	close(connect[1]);
	if (fds[0] != STDIN_FILENO)
			close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
	close(fd_in);
	return (connect[0]);
}

void	final_command(int *count, t_token **tokens, int fd_in)
{
	pid_t	pid;
	int 	fds[2];
	char 	**args;
	int		original_stdout;
	int		status;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections((*tokens), &fds, *count);
	args = build_command_string((*tokens), count);
	if (!args || !args[0])
		return ;
	signals('c');
	if (ft_strncmp(args[0] , "./minishell", 12) == 0)
		ign_signal();
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free_array(args);
		close(fd_in);
		return ;
	}
	else if (pid == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		if (fd_in < 0)
		{
			perror("error output file");
			exit(7);
		}
		original_stdout = dup(STDOUT_FILENO);
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
		if (is_builtin(args) == 1)
		{
			handle_builtin(args, *tokens);
			close(fds[1]);
			exit(0);
		}
		else
		{
			exe((*tokens), args, original_stdout);
			exit(EXIT_FAILURE);
		}
	}
	waitpid(pid, &status, 0); 
	exit_num = WEXITSTATUS(status);
	signals('f');
	close(fd_in);
	free_array(args);
}

int	first_command(t_token **tokens, int num_commands, int *count)
{
	int		fds[2];
	int		connect[2];
	char	**args;
	pid_t	pid;
	int 	i;
	int		original_stdin;
	int		original_stdout;
	int status;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(*tokens, &fds, *count); //tokens
	args = build_command_string(*tokens, count); //tokens
	if (!args || !args[0])
		return (ERROR);
	if (is_builtin(args) == 1 && num_commands == 1)
	{
		original_stdin = -1;
		original_stdout = -1;
		if (fds[0] != STDIN_FILENO)
		{
			original_stdin = dup(STDIN_FILENO);
			dup2(fds[0], STDIN_FILENO);
		}
		if (fds[1] != STDOUT_FILENO)
		{
			original_stdout = dup(STDOUT_FILENO);
			dup2(fds[1], STDOUT_FILENO);
		}
		handle_builtin(args, *tokens); //tokens
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
		if (fds[0] != STDIN_FILENO) //arriba
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
		return (STDOUT_FILENO);
	}
	if (num_commands == 1)
	{
		signals('c');
		if (ft_strncmp(args[0] , "./minishell", 12) == 0)
			ign_signal();
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (ERROR);
		}
		else if (pid == 0) // Proceso hijo
		{
			if (ft_strncmp(args[0] , "./minishell", 12) == 0)
				modify_shlvl(tokens, "SHLVL");
			original_stdout = dup(STDOUT_FILENO);
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
			exe((*tokens), args, original_stdout);
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
		waitpid(pid, &status, 0);
		exit_num = WEXITSTATUS(status);
		signals('f');
		return (STDOUT_FILENO);
	}
	else
	{
		if (pipe(connect) == -1)
		{
			perror("pipe");
			return (ERROR);
		}
		if (ft_strncmp(args[0] , "./minishell", 12) == 0)
		{
			close(connect[1]);
			return (connect[0]);
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
			original_stdout = dup(STDOUT_FILENO);
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
			if (is_builtin(args) == 1)
			{
				handle_builtin(args, *tokens);
				close(fds[1]);
				exit(0);
			}
			else
			{
				exe((*tokens), args, original_stdout);
				exit(EXIT_FAILURE);
			}
		}
		free_array(args);
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
	int		num_commands;
	int		i;
	char	**env;
	int		count;
	int		status;

	env = join_env(tokens->env_mshell);
	if (!env)
		return (ERROR);
	num_commands = num_pipes(argv_str) + 1;
	count = 0;
	fd_in = first_command(&tokens, num_commands, &count);
	if (fd_in < 0)
	{
		free_array(env);
		return (ERROR);
	}
	if (num_commands == 1)
	{
		if (fd_in != STDOUT_FILENO)
			close(fd_in);
		waitpid(-1, &status, 0);
		free_array(env);
		return (0);
	}
	if (num_commands > 1)
	{
		count = 1;
		i = 1;
		while (i < num_commands - 1)
		{
			fd_in = middle_command(&count, &tokens, fd_in);
			count++;
			i++;
		}
		if (i < num_commands)
			final_command(&count, &tokens, fd_in);
		i = 0;
		while (i < num_commands)
		{
			waitpid(-1, &status, 0);
			i++;
		}
	}
	free_array(env);
	return (0);
}
