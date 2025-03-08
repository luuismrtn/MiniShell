/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/08 20:41:12 by adrianafern      ###   ########.fr       */
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
	};                            // w  f  |  <  > <<  >> $
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
		return (1);
	}
	else
	{
		printf("correct\n\n\n");
		return (0);
	}
	return (1);
}

void	setup_redirections(t_token *tokens, int (*fds)[2], int count)
{
	int	new_fd;
	int aux_move;

	t_token *temp_tokens; // probar sin temporal
	temp_tokens = tokens->next;
	aux_move = count;
	while (aux_move > 0)
	{
		if (temp_tokens->type == T_PIPE)
			aux_move--;
		temp_tokens = temp_tokens->next;
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
			break ;
		temp_tokens = temp_tokens->next;
	}
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_WORD || tokens->type == T_FLAG
			|| tokens->type == T_ENV)
			count++;
		if (tokens->type == T_PIPE)
			break ;
		tokens = tokens->next;
	}
	return (count);
}

char	**build_command_string(t_token *tokens, int *count)
{
	char	**args;
	int		num_args;
	int		i;
	t_token	*temp_tokens; //esto se podria quitar
	int aux_move;

	temp_tokens = tokens->next;
	aux_move = (*count);
	while (aux_move > 0)
	{
		if (temp_tokens->type == T_PIPE)
		{
			(*count)++;
			aux_move--;
		}
		temp_tokens = temp_tokens->next;
	}
	printf("count comds; %d\n", (*count));
	num_args = count_args(temp_tokens);
	printf("num_Args %d\n", num_args);
	args = malloc((num_args + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while ((i < num_args) && temp_tokens)
	{
		if (temp_tokens->type == T_WORD || temp_tokens->type == T_FLAG
			|| temp_tokens->type == T_ENV)
		{
			args[i] = ft_strdup(temp_tokens->content);
			if (!args[i])
				return (free(args), NULL);
			i++;
		}
		if (temp_tokens && temp_tokens->type == T_PIPE)
			break ;
		if (temp_tokens->type == T_REDIR_RIGHT || temp_tokens->type == T_APPEND
			|| temp_tokens->type == T_REDIR_LEFT)
		{
			temp_tokens = temp_tokens->next->next;
			continue ;
		}
		temp_tokens = temp_tokens->next;
	}
	args[i] = NULL;
	return (args);
}

static void	execute_command(char **full_command, t_token *tokens, int fd_in,
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
		handle_builtin(full_command, tokens);
	else
		exe(join_env(tokens->env_mshell), full_command);
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
		if (exit_num == 1)
			exit_num = 0;
	}
	i = 0;
	while (full_command[i])
		free(full_command[i++]);
	free(full_command);
}

static void	child_process(char **full_command, t_token *tokens, int fd_in,
		int fd_out)
{
	int	i;

	execute_command(full_command, tokens, fd_in, fd_out);
	i = 0;
	while (full_command[i])
		free(full_command[i++]);
	free(full_command);
	exit(1);
}

void	make_exe_command(t_token *tokens)
{
	char	**full_command;
	pid_t	pid;
	int		fds[2];
	int		i;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	setup_redirections(tokens, &fds, 0);
	full_command = build_command_string(tokens, 0);
	if (!full_command)
		return ;
	printf("full command\n");
	print_2(full_command);
	i = 0;
	while (full_command[i] != NULL)
	{
		printf("full command [%d]  %s\n", i, full_command[i]);
		i++;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		clean_resources(full_command, fds[0], fds[1]);
		return ;
	}
	else if (pid == 0)
		child_process(full_command, tokens, fds[0], fds[1]);
	else
		parent_process(pid, full_command, fds[0], fds[1]);
}
