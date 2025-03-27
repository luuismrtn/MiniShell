/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:31:15 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/27 16:31:04 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	expand_in_heredoc(char **line, t_token *tokens)
// {
// 	int		i;
// 	char	*var_content;
// 	char	*content;
// 	char	*temp;
// 	int		start;

// 	i = 0;
// 	content = NULL;
// 	while (line[0][i])
// 	{
// 		if (line[0][i] == '$')
// 		{
// 			if (content == NULL)
// 				content = ft_substr(line[0], 0, i);
// 			else
// 				content = ft_strjoin(temp, ft_substr(line[0], start, i - start));
// 			i++;
// 			if (line[0][i] == '?')
// 			{
// 				i++;
// 				temp = ft_strjoin(content, ft_itoa(exit_num));
// 				free(content);
// 				start = i;
// 			}
// 			else
// 			{
// 				var_content = get_env_content(tokens, *line, i, &start);
// 				temp = ft_strjoin(content, var_content);
// 				free(content);
// 				free(var_content);
// 			}
// 		}
// 		i++;
// 	}
// 	(*line) = ft_strjoin(temp, ft_substr(line[0], start, i - start));
// 	free(temp);
// }

int	fnd_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && !ft_isspace(str[i + 1]))
			return (0);
		i++;
	}
	return (1);
}

void	handle_heredoc(char *eof, int fd, t_token *tokens)
{
	char	*line;
	char	*r_lines;
	char	*temp;

	r_lines = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, eof, ft_strlen(eof) + 1) == 0)
			break ;
		if (tokens && tokens->next && tokens->next->next && tokens->next->next->next)
		{
			if (fnd_dollar(line) == 0 && tokens->next->next->next->quotes == 0)
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
