/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:57:25 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/05 23:56:01 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_read_history(char *history_file)
{
	int		fd;
	char	*line;

	fd = open(history_file, O_CREAT | O_RDONLY, 0644);
	if (fd < 0)
	{
		perror("error opening history file");
		return (ERROR);
	}
	line = get_next_line(fd);
	while (line)
	{
		add_history(ft_strtrim(line, "\n"));
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (SUCCESS);
}

char	*return_last_command(int fd)
{
	char	*a_line;
	char	*p_line;

	a_line = get_next_line(fd);
	if (a_line == NULL)
		return (NULL);
	p_line = a_line;
	while (a_line != NULL)
	{
		p_line = a_line;
		a_line = get_next_line(fd);
	}
	return (p_line);
}

int	write_line_history(char *history_file, char *line)
{
	int		fd;
	char	*line_clean;
	char	*last_command;

	fd = open(history_file, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("error opening history file");
		return (ERROR);
	}
	line_clean = ft_strtrim(line, " ");
	if (!line || !line_clean[0])
	{
		close(fd);
		return (ERROR);
	}
	last_command = return_last_command(fd);
	if (last_command == NULL)
	{
		write(fd, line_clean, ft_strlen(line_clean));
		free(line_clean);
		close(fd);
	}
	else
	{
		if (ft_strncmp(line_clean, last_command, ft_strlen(line_clean)
				+ ft_strlen(last_command)) != 0)
		{
			if (line_clean[0] != '\0')
				write(fd, "\n", 1);
			write(fd, line_clean, ft_strlen(line_clean));
			free(line_clean);
			close(fd);
		}
	}
	return (SUCCESS);
}
