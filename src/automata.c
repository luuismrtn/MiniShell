/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/05 20:30:04 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	automata(t_token *tokens)
{
	int		current_state;
	int		prev_token;
	char	*elements[9] = {"command", "flag", "|", "newline", "newline",
			"newline", "newline", "err"};

	tokens = tokens->next;
	int automata[6][8] = {
		{1, 5, 5, 3, 3, 3, 3, 2}, // inicial
		{2, 1, 4, 3, 3, 3, 3, 1}, // comando
		{2, 2, 4, 3, 3, 3, 3, 2}, // file
		{2, 2, 5, 5, 5, 5, 5, 2}, // redireccion
		{1, 5, 5, 3, 3, 3, 3, 2}, // pipe
		{5, 5, 5, 5, 5, 5, 5, 5}  // err
	};// w  f  |  <  > <<  >> $
	current_state = 0;
	while (current_state != 5 && tokens != NULL)
	{
		prev_token = tokens->type;
		current_state = automata[current_state][tokens->type];
		tokens = tokens->next;
	}
	if (current_state != 1 && current_state != 2)
	{
		printf("syntax error: %s\n\n", elements[prev_token]);
		printf("END\n\n");
		return 1;
	}
	else
	{
		printf("correct\n\n\n");
		return 0;
	}
	return 1;
}

void	setup_redirections(t_token *tokens, int (*fds)[2], int num_comnd, int *count)
{
	t_token	*temp_tokens; //probar sin temporal
	int		new_fd;

	temp_tokens = tokens->next;
	if (num_comnd > 1)
	{
		while (temp_tokens && (*count) < num_comnd)
		{
			if (temp_tokens->type == T_PIPE)
				(*count)++;
			temp_tokens = temp_tokens->next;
		}
	}
	while (temp_tokens)
	{
		if (temp_tokens->type == T_REDIR_RIGHT && temp_tokens->next)
		{
			new_fd = open(temp_tokens->next->content,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (new_fd == -1)
			{
				perror("minishell: open");
				return ;
			}
			if ((*fds)[1] != STDOUT_FILENO)
				close((*fds)[1]);
			(*fds)[1] = new_fd;
		}
		else if (temp_tokens->type == T_APPEND && temp_tokens->next)
		{
			new_fd = open(temp_tokens->next->content,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (new_fd == -1)
			{
				perror("minishell: open");
				return ;
			}
			if ((*fds)[1] != STDOUT_FILENO)
				close((*fds)[1]);
			(*fds)[1] = new_fd;
		}
		else if (temp_tokens->type == T_REDIR_LEFT && temp_tokens->next)
		{
			new_fd = open(temp_tokens->next->content, O_RDONLY);
			if (new_fd == -1)
			{
				perror("minishell: open");
				return ;
			}
			if ((*fds)[0] != STDIN_FILENO)
				close((*fds)[0]);
			(*fds)[0] = new_fd;
		}
		else if (temp_tokens && temp_tokens->type == T_PIPE)
			break;
		temp_tokens = temp_tokens->next;
	}
}

char	*build_command_string(t_token *tokens, int num_comnd, int *count)
{
	char	*args;
	char	*temp1;
	t_token	*temp_tokens; //probar sin temporal

	args = ft_strdup("");
	if (!args)
		return (NULL);
	temp_tokens = tokens->next;
	if (num_comnd > 1)
	{
		while (temp_tokens && (*count) < num_comnd)
		{
			if (temp_tokens->type == T_PIPE)
				(*count)++;
			temp_tokens = temp_tokens->next;
		}
	}
	while (temp_tokens != NULL)
	{
		if (temp_tokens->type == T_REDIR_RIGHT || temp_tokens->type == T_APPEND
			|| temp_tokens->type == T_REDIR_LEFT)
		{
			temp_tokens = temp_tokens->next->next;
			continue ;
		}
		temp1 = args;
		args = ft_strjoin(args, temp_tokens->content);
		free(temp1);
		temp1 = args;
		args = ft_strjoin(args, " ");
		free(temp1);
		temp_tokens = temp_tokens->next;
		if (temp_tokens && temp_tokens->type == T_PIPE)
			break;
	}
	return (args);
}

static void	execute_command(char **full_command, char **env, int fd_in,
		int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (ft_strncmp(full_command[0], "echo", 5) == 0
		|| ft_strncmp(full_command[0], "cd", 3) == 0
		|| ft_strncmp(full_command[0], "pwd", 4) == 0
		|| ft_strncmp(full_command[0], "env", 4) == 0
		|| ft_strncmp(full_command[0], "export", 7) == 0
		|| ft_strncmp(full_command[0], "unset", 6) == 0)
		handle_builtin(full_command, env);
	else
		exe(env, full_command);
}

static void	clean_resources(char **full_command, int fd_in, int fd_out)
{
	int	i;

	i = 0;
	while (full_command[i])
		free(full_command[i++]);
	free(full_command);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
}

static void	parent_process(pid_t pid, char **full_command, int fd_in,
		int fd_out)
{
	int	status;
	int	i;

	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) 
	{
        exit_num = WEXITSTATUS(status);
		if (exit_num == 1) //si hijo ok, devuelve 1
			exit_num = 0;
	}
	i = 0;
	while (full_command[i])
		free(full_command[i++]);
	free(full_command);
}

static void	child_process(char **full_command, char **env, int fd_in,
		int fd_out)
{
	int	i;

	execute_command(full_command, env, fd_in, fd_out);
	i = 0;
	while (full_command[i])
		free(full_command[i++]);
	free(full_command);
	exit(1);
}

void	make_exe_command(t_token *tokens, char **env)
{
	char	**full_command;
	pid_t	pid;
	int		fd_in;
	int		fd_out;
	char	*args;
	int		i;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	setup_redirections(tokens, &fd_in, &fd_out);
	args = build_command_string(tokens); 
	if (!args)
		return ;
	full_command = ft_split(args, ' '); 
	free(args);
	if (!full_command)
		return ;
	i = 0;
	while (full_command[i] != NULL)
	{
		printf("full command [%d]  %s\n", i, full_command[i]);
		i++;
	}
	if (ft_strncmp(full_command[0], "exit", 5) == 0) //y no pipes
		ft_exit(&args);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		clean_resources(full_command, fd_in, fd_out);
		return ;
	}
	else if (pid == 0)
		child_process(full_command, env, fd_in, fd_out);
	else
		parent_process(pid, full_command, fd_in, fd_out);
}
