/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 21:34:31 by lumartin         ###   ########.fr       */
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
		return (1);
	}
	else
	{
		return (0);
	}
	return (1);
}

void expand_in_heredoc(char **line, t_token *tokens)
{
	int i;
	int len_var_name;
	char *var_name;
	char *var_content;
	char *content;
	char *temp;
	int start_after_$;
	t_env *current_env_list;

	i = 0;
	while (line[0][i])
	{
		if (line[0][i] == '$')
		{
			content = ft_substr(line[0], 0, i);
			i++;
			if (line[0][i] == '?')
			{
				i++;
				temp = ft_strjoin(content, ft_itoa(exit_num));
				free(content);
				start_after_$ = i;
			}
			else
			{
				len_var_name = ft_len_var_name(line[0], i);
				var_name = ft_substr(line[0], i, len_var_name);
				current_env_list = tokens->env_mshell;
				while (current_env_list != NULL)
				{
					if (ft_strncmp(current_env_list->name, var_name,
							len_var_name) == SUCCESS)
					{
						free(var_name);
						var_content = ft_strdup(current_env_list->content);
						break ;
					}
					current_env_list = current_env_list->next;
				}
				temp = ft_strjoin(content, var_content);
				free(var_content);
				free(content);
				start_after_$ = i + len_var_name;
			}
		}
		i++;
	}
	(*line) = ft_strjoin(temp, ft_substr(line[0], start_after_$, i - start_after_$));
	free(temp);
}

int find_the_dollar(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && !ft_isspace(str[i + 1]))
			return 0;
		i++;
	}
	return 1;
}

void write_in_pipe(char *content, int *fd)
{
	int 	connect[2];
	
	if (pipe(connect) == -1)
	{
		perror("pipe");
		return;
	}
	write(connect[1], content, ft_strlen(content));
	if ((*fd) != STDIN_FILENO)
		close((*fd));
	(*fd) = connect[0];
	close(connect[1]);
}

void	handle_heredoc(char **eof, int *fd, t_token *tokens)
{
	char	*line;
	char	*r_lines;
	char 	*temp;

	line = readline("> ");
	if (!line)
		return;
	if (ft_strncmp(line, *eof, ft_strlen(*eof) + 1) == 0)
	{
		write_in_pipe("", fd);
		return;
	}
	if (find_the_dollar(line) == SUCCESS)
		expand_in_heredoc(&line, tokens);
	r_lines = ft_strjoin(line, "\n");
	while ((ft_strlen(line) != ft_strlen(*eof)) || ft_strcmp(line, *eof) != 0)
	{
		free(line);
		line = readline("> ");
		if (!line || ft_strcmp(line, *eof) == 0)
			break;
		if (find_the_dollar(line) == SUCCESS)
			expand_in_heredoc(&line, tokens);
		temp = ft_strjoin(r_lines, line);
		free(r_lines);
		r_lines = ft_strjoin(temp, "\n");
		free(temp);
	}
	write_in_pipe(r_lines, fd);
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
			handle_heredoc(&temp_tokens->next->content, fds[0], tokens);
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
	num_args = count_args(temp_tokens);
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
