/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:31:15 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/01 21:30:26 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Detecta si una cadena contiene un símbolo de dólar para expansión
 *
 * Esta función analiza una cadena para determinar si contiene
 * un símbolo de dólar ($) seguido por un carácter que no sea espacio,
 * lo que indicaría una variable a expandir.
 *
 * @param str Cadena a analizar
 * @return int 0 si se encuentra un símbolo $ seguido de un carácter no-espacio,
 *             1 si no se encuentra ningún patrón de expansión
 */
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

/**
 * @brief Procesa un heredoc, leyendo líneas hasta encontrar el delimitador
 *
 * Esta función implementa el comportamiento del heredoc (<<) en la shell.
 * Lee líneas desde la entrada estándar hasta encontrar el delimitador (eof),
 * y las escribe en el descriptor de archivo proporcionado.
 *
 * Características:
 * - Lee líneas de la entrada con el prompt ">"
 * - Termina al encontrar EOF o una línea que coincida exactamente con el
 * delimitador
 * - Si el delimitador no estaba entre comillas y la línea contiene $,
	expande variables
 * - Concatena todas las líneas leídas y las escribe en el descriptor
 proporcionado
 *
 * @param eof Delimitador que marca el final del heredoc
 * @param fd Descriptor de archivo donde escribir el contenido del heredoc
 * @param tokens Puntero a la lista de tokens para acceso al entorno (puede ser
 * NULL)
 */
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
		if (tokens && tokens->next && tokens->next->next
			&& tokens->next->next->next)
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
