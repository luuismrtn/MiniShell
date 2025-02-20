/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 19:03:15 by lumartin          #+#    #+#             */
/*   Updated: 2024/09/28 19:30:09 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*initialize_buffers(char **buffer)
{
	char	*read_buffer;

	read_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	if (!(*buffer))
	{
		*buffer = (char *)malloc(1);
		if (!(*buffer))
		{
			free(read_buffer);
			return (NULL);
		}
		(*buffer)[0] = '\0';
	}
	return (read_buffer);
}

static char	*read_line_from_buffer(char **buffer, char *read_buffer)
{
	void	*line;
	char	*temp;
	int		i;

	i = 0;
	line = NULL;
	while ((*buffer)[i])
	{
		if ((*buffer)[i] == '\n')
		{
			line = ft_strjoin_gnl(line, *buffer, i + 1);
			temp = *buffer;
			*buffer = ft_strjoin_gnl(NULL, *buffer + i + 1, ft_strlen(*buffer
						+ i + 1));
			free(temp);
			free(read_buffer);
			return (line);
		}
		i++;
	}
	return (NULL);
}

static char	*handle_end_of_file(char **buffer, char *read_buffer)
{
	void	*line;

	line = NULL;
	if (ft_strlen(*buffer) > 0)
		line = ft_strjoin_gnl(line, *buffer, ft_strlen(*buffer));
	free(*buffer);
	*buffer = NULL;
	free(read_buffer);
	return (line);
}

static char	*handle_read_error(char **buffer, char *read_buffer)
{
	free(*buffer);
	*buffer = NULL;
	free(read_buffer);
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*buffers[MAX_FD] = {NULL};
	void		*line;
	char		*read_buffer;
	int			bytes_read;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	read_buffer = initialize_buffers(&buffers[fd]);
	if (!read_buffer)
		return (NULL);
	while (1)
	{
		line = read_line_from_buffer(&buffers[fd], read_buffer);
		if (line)
			return (line);
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (handle_read_error(&buffers[fd], read_buffer));
		if (bytes_read == 0)
			return (handle_end_of_file(&buffers[fd], read_buffer));
		read_buffer[bytes_read] = '\0';
		buffers[fd] = ft_strjoin_gnl(buffers[fd], read_buffer, bytes_read);
	}
}
