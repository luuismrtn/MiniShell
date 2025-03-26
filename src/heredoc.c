/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:31:15 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/27 00:05:23 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char *get_env_content(t_token *tokens, char *line, int i, int *start_after_$)
{
	int len_var_name;
	char *var_name;
	char *var_content;
	t_env	*current;

	len_var_name = ft_len_var_name(line, i);
	var_name = ft_substr(line, i, len_var_name);
	current = tokens->env_mshell;
	while (current)
	{
		if (ft_strncmp(current->name, var_name, len_var_name) == SUCCESS)
		{
			free(var_name);
			var_content = ft_strdup(current->content);
			break ;
		}
		current = current->next;
	}
	(*start_after_$) = i + len_var_name;
	return(var_content);
}

void expand_in_heredoc(char **line, t_token *tokens)
{
	int i;
	char *var_content;
	char *content;
	char *temp;
	int start_after_$;

	i = 0;
	content = NULL;
	while (line[0][i])
	{
		if (line[0][i] == '$')
		{
			if (content == NULL)
				content = ft_substr(line[0], 0, i);
			else
				content = ft_strjoin(temp, ft_substr(line[0], start_after_$, i - start_after_$));
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
				var_content = get_env_content(tokens, *line, i, &start_after_$);
				temp = ft_strjoin(content, var_content);
				free(content);
				free(var_content);
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

void	handle_heredoc(char *eof, int fd, t_token *tokens)
{
	char	*line;
	char	*r_lines;
	char 	*temp;

	r_lines = ft_strdup("");
	while ((ft_strlen(line) != ft_strlen(eof)) || ft_strcmp(line, eof) != 0)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
			break;
		if (tokens && tokens->next && tokens->next->next && tokens->next->next->next)
		{
			if (find_the_dollar(line) == SUCCESS && tokens->next->next->next->quotes == 0)
				expand_in_heredoc(&line, tokens);	
		}
		temp = ft_strjoin(r_lines, line);
		free(r_lines);
		r_lines = ft_strjoin(temp, "\n");
		free(temp);
		free(line);
	}
	write(fd, r_lines, ft_strlen(r_lines));
	free(line);
	free(r_lines);
}
