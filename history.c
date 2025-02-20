/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:57:25 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/20 17:51:13 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

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
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
    close(fd);
	return (SUCCESS);
}

char *return_last_command(char *history_file, int fd)
{
	char *a_line;
	char *p_line;

	a_line = get_next_line(fd);
	if (a_line == NULL)
		return (0);
	p_line = a_line;
	while (a_line != NULL)
	{
		p_line = a_line;
		a_line = get_next_line(fd);
	}
	return (p_line);
}

int	write_line_history(char *history_file, char* line)
{
	int		fd;
	char 	*line_clean;

	fd = open(history_file, O_CREAT | O_WRONLY | O_APPEND, 0200);
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
	ft_putstr_fd(return_last_command(line_clean, fd), 1);
	if (ft_strncmp(line_clean, return_last_command(line_clean, fd), ft_strlen(line_clean)) == 0)
	{
		write(fd, line_clean, ft_strlen(line_clean));
		write(fd, "\n", 1);
		free(line_clean);
		close(fd);
	}
	return (SUCCESS);
}
