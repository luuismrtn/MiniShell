/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:19:18 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/02 16:01:30 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Cuenta el número de líneas en el archivo de historial
 *
 * Esta función cuenta el número de líneas en el archivo de historial.
 * 
 * @param history_file Ruta al archivo de historial
 * @return int Número de líneas en el archivo, o -1 si hay error
 */
static int	count_history_lines(char *history_file)
{
	int		fd;
	char	*line;
	int		count;

	fd = open(history_file, O_RDONLY);
	if (fd < 0)
		return (-1);
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

/**
 * @brief Lee el historial de comandos desde un archivo
 * 
 * Esta función lee el historial de comandos desde un archivo.
 *
 * @param history_file Ruta al archivo de historial
 * @return Un puntero al array o NULL en caso de error
 */
char	**get_history(char *history_file)
{
	char	**history;
	int		fd;
	int		i;
	int		count;
	char	*line;

	fd = open(history_file, O_RDONLY);
	count = count_history_lines(history_file);
	history = malloc(sizeof(char *) * (HISTORY_MAX_LINES + 1));
	if (!history || count < 0 || fd < 0)
		return (NULL);
	i = 0;
	while (count > HISTORY_MAX_LINES)
	{
		line = get_next_line(fd);
		free(line);
		count--;
	}
	while (i < count)
		history[i++] = get_next_line(fd);
	history[i] = NULL;
	close(fd);
	return (history);
}

/**
 * @brief Reemplaza el historial en el archivo con el nuevo contenido
 *
 * Esta función reemplaza el contenido del archivo de historial
 * 
 * @param history_file Ruta al archivo de historial
 * @param history Array de cadenas con el nuevo contenido del historial
 * @return int SUCCESS o ERROR
 */
int	replace_history(char *history_file, char **history)
{
	int	fd;
	int	i;

	if (!history)
		return (ERROR);
	fd = open(history_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		free_array(history);
		return (ERROR);
	}
	i = 0;
	while (history[i])
	{
		write(fd, history[i], ft_strlen(history[i]));
		free(history[i]);
		i++;
	}
	free(history);
	close(fd);
	return (SUCCESS);
}
