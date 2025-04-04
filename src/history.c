/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:57:25 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/04 12:05:21 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Lee el historial de comandos desde un archivo
 * 
 * Esta función lee el historial de comandos desde un archivo y lo
 * añade a la lista de historial de readline.
 *
 * @param history_file Ruta al archivo de historial
 * @return int SUCCESS o ERROR
 */
int	ft_read_history(char *history_file)
{
	int		fd;
	char	*line;
	char	*trimmed_line;

	fd = open(history_file, O_CREAT | O_RDONLY, 0644);
	if (fd < 0)
	{
		perror("error opening history file");
		return (ERROR);
	}
	line = get_next_line(fd);
	while (line)
	{
		trimmed_line = ft_strtrim(line, "\n");
		add_history(trimmed_line);
		free(trimmed_line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (SUCCESS);
}

/**
 * @brief Obtiene la última línea del archivo de historial.
 * 
 * Esta función lee el archivo de historial y devuelve la última línea.
 *
 * @param fd Descriptor del archivo de historial
 * @return char* Última línea o NULL
 */
char	*return_last_command(int fd)
{
	char	*current_line;
	char	*last_line;
	int		read_fd;

	read_fd = dup(fd);
	if (read_fd < 0)
		return (NULL);
	last_line = NULL;
	current_line = get_next_line(read_fd);
	while (current_line != NULL)
	{
		if (last_line != NULL)
			free(last_line);
		last_line = current_line;
		current_line = get_next_line(read_fd);
	}
	close(read_fd);
	return (last_line);
}

/**
 * @brief Escribe la línea en el archivo de historial si no es duplicada
 * 
 * Esta función escribe la línea limpia en el archivo de historial
 * si no es una duplicada de la última línea.
 *
 * @param fd Descriptor del archivo de historial
 * @param line_clean Línea limpia a escribir
 * @param last_command Última línea del archivo de historial
 * @return int SUCCESS en todos los casos
 */
static int	process_history_line(int fd, char *line_clean, char *last_command)
{
	if (last_command == NULL)
	{
		write(fd, line_clean, ft_strlen(line_clean));
		free(line_clean);
		close(fd);
	}
	else
	{
		if (match_string(line_clean, last_command) == 0)
		{
			if (line_clean[0] != '\0')
				write(fd, "\n", 1);
			write(fd, line_clean, ft_strlen(line_clean));
			add_history(line_clean);
		}
		free(last_command);
		free(line_clean);
		close(fd);
	}
	return (SUCCESS);
}

/**
 * @brief Escribe una línea en el archivo de historial
 * 
 * Esta función escribe una línea en el archivo de historial
 *
 * @param history_file Ruta al archivo de historial
 * @param line Línea a escribir en el historial
 * @return int SUCCESS si se escribió correctamente, ERROR en caso contrario
 */
int	write_line_history(char *history_file, char *line)
{
	int		fd;
	char	*line_clean;
	char	*last_command;
	char	**history;

	history = get_history(history_file);
	if (history)
		replace_history(history_file, history);
	fd = open(history_file, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("error opening history file");
		return (ERROR);
	}
	line_clean = ft_strtrim(line, " ");
	if (!line || !line_clean || !line_clean[0])
	{
		free(line_clean);
		close(fd);
		return (ERROR);
	}
	last_command = return_last_command(fd);
	return (process_history_line(fd, line_clean, last_command));
}
