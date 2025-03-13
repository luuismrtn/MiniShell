/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 15:23:06 by aldferna         ###   ########.fr       */
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

void	handle_heredoc(char **content, int *fd)
{
	char	*line;
	char	*r_lines;
	char 	*temp;
	int 	connect[2];

	line = readline("> ");
	if (!line)
		return;
	r_lines = ft_strjoin(line, "\n");
	while ((ft_strlen(line) != ft_strlen(*content)) || ft_strcmp(line, *content) != 0)
	{
		free(line);
		line = readline("> ");
		if (!line || ft_strcmp(line, *content) == 0)
			break;
		temp = ft_strjoin(r_lines, line);
		free(r_lines);
		r_lines = ft_strjoin(temp, "\n");
		free(temp);
	}
	printf("r_lines: %s\n", r_lines);
	if (pipe(connect) == -1)
	{
		perror("pipe");
		return;
	}
	write(connect[1], r_lines, ft_strlen(r_lines));
	if ((*fd) != STDIN_FILENO)
		close((*fd));
	(*fd) = connect[0];
	close(connect[1]);
	free(line);
	free(r_lines);
}

void	setup_redirections(t_token *tokens, int (*fds)[2], int count)
{
	int	new_fd;
	int	aux_move;

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
		else if (temp_tokens->type == T_HERE_DOC && temp_tokens->next)
		{
			handle_heredoc(&temp_tokens->next->content, fds[0]);
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
	int		aux_move;

	t_token *temp_tokens; // esto se podria quitar
	temp_tokens = tokens->next;
	aux_move = (*count);
	while (aux_move > 0)
	{
		if (temp_tokens->type == T_PIPE)
			aux_move--;
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
			|| temp_tokens->type == T_REDIR_LEFT || temp_tokens->type == T_HERE_DOC)
		{
			temp_tokens = temp_tokens->next->next;
			continue ;
		}
		temp_tokens = temp_tokens->next;
	}
	args[i] = NULL;
	return (args);
}
