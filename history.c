/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:57:25 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/20 16:20:41 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	read_history(char *history_file)
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

int	write_line_history(char *history_file, char* line)
{
	int		fd;

	fd = open(history_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("error opening history file");
		return (ERROR);
	}
    if (!line || !line[0] || line)
    {
        close(fd);
        return (ERROR);
    }
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
    close(fd);
	return (SUCCESS);
}
