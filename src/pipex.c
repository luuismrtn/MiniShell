/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/24 21:06:38 by adrianafern      ###   ########.fr       */
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

void	modify_shlvl(t_token **tokens, char *var)
{
	t_env	*aux;
	t_env	*new_var;
	int		shlvl;

	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (strncmp(aux->name, var, strlen(var)) == 0)
		{
			shlvl = ft_atoi(aux->content) + 1;
			free(aux->content);
			aux->content = ft_itoa(shlvl);
			return ;
		}
		aux = aux->next;
	}
	shlvl = 1;
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdup(var);
	new_var->content = ft_itoa(shlvl);
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}

void change_fds_redir(int (*fds)[2], int *o_stdin, int *o_stdout, int builtin_father)
{
	if ((*fds)[0] != STDIN_FILENO)
	{
		if (builtin_father == 1)
			(*o_stdin) = dup(STDIN_FILENO);
		dup2((*fds)[0], STDIN_FILENO);
		close((*fds)[0]);
	}
	if ((*fds)[1] != STDOUT_FILENO)
	{
		if (builtin_father == 1)
			(*o_stdout) = dup(STDOUT_FILENO);
		dup2((*fds)[1], STDOUT_FILENO);
		close((*fds)[1]);
	}
}

void executor(t_tokens *tokens, int (*fds)[2], char **args, int original_stdout)
{
	int		original_stdin;
	
	if (is_builtin(args) == 1)
	{
		original_stdin = -1;
		change_fds_redir(fds, &original_stdin, &original_stdout, 1);
		handle_builtin(args, *tokens);
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
	}
	else
	{
		change_fds_redir(fds, NULL, NULL, 0);
		exe(tokens, args, original_stdout);
	}
}

void clean_father_material(int (*fds)[2], char ***args)
{
	if ((*fds)[0] != STDIN_FILENO)
		close((*fds)[0]);
	if ((*fds)[1] != STDOUT_FILENO)
		close((*fds)[1]);
	free_array((*args));
}

void error_pipe_fork(int *pipe_in, int *pipe_out, char ***args, char c)
{
	free_array((*args));
	if (char == 'p')
		perror("pipe");
	if (char == 'f')
	{
		perror("fork");
		close((*pipe_in));
		close((*pipe_out));
	}
	if (char == 'e')
	{
		perror("fork");
		close((*pipe_in));
	}
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
	args = build_command_string((*tokens), count);
	if (!args || !args[0])
		return (ERROR);
	if (pipe(connect) == -1)
		return (error_pipe_fork(NULL, NULL, &args, 'p'), ERROR);
	pid = fork();
	if (pid == -1)
		return (error_pipe_fork(&connect[0], &connect[1], &args, 'f'), ERROR);
	else if (pid == 0)
	{
		if (fd_in < 0)
		{
			perror("fd_in\n");
			exit(1);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		close(connect[0]);
		original_stdout = dup(STDOUT_FILENO); 
		dup2(connect[1], STDOUT_FILENO);
		close(connect[1]);
		executor((*tokens), &fds, args, original_stdout);
		exit(exit_num);
	} 
	close(connect[1]);
	clean_father_material(&fds, &args);
	close(fd_in);
	return (connect[0]);
}

void	final_command(int *count, t_token **tokens, int fd_in)
{
	pid_t	pid;
	int		fds[2];
	char	**args;
	int status;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections((*tokens), &fds, *count);
	args = build_command_string((*tokens), count);
	if (!args || !args[0])
		return ;
	signals('c');
	if (ft_strncmp(args[0], "./minishell", 12) == 0)
		ign_signal();
	pid = fork();
	if (pid == -1)
		return (error_pipe_fork(&fd_in, NULL, &args, 'e'), ERROR);
	else if (pid == 0)
	{
		if (fd_in < 0)
		{
			perror("error output file");
			exit(1);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		executor((*tokens), &fds, args, dup(STDOUT_FILENO));
		exit(exit_num);
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
	int		original_stdout;
	int		status;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(*tokens, &fds, *count);
	args = build_command_string(*tokens, count);
	if (!args || !args[0])
		return (ERROR);
	if (pipe(connect) == -1)
		return (error_pipe_fork(NULL, NULL, &args, 'p'), ERROR);
	if (ft_strncmp(args[0], "./minishell", 12) == 0)
		return (close(connect[1]), connect[0]);
	pid = fork();
	if (pid == -1)	
		return (error_pipe_fork(&connect[0], &connect[1], &args, 'f'), ERROR);
	else if (pid == 0)
	{
		close(connect[0]);
		original_stdout = dup(STDOUT_FILENO);
		dup2(connect[1], STDOUT_FILENO);
		close(connect[1]);
		executor((*tokens), &fds, args, original_stdout);
		exit(exit_num);
	}
	clean_father_material(&fds, &args);
	return (close(connect[1]), connect[0]);
}

int	pipex(char *argv_str, t_token *tokens, int num_commands)
{
	int		fd_in;
	int		i;
	int		count;
	int		status;

	count = 0;
	fd_in = first_command(&tokens, num_commands, &count);
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
	return (0);
}
