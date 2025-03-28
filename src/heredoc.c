/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:31:15 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/28 12:32:39 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
