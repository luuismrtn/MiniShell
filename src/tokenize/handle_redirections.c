/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:02:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/03 21:29:54 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Procesa un operador de heredoc (<<) en la entrada
 *
 * Identifica un operador heredoc (<<) y crea los tokens correspondientes.
 * Avanza el índice de lectura y añade el token para el operador de heredoc.
 * Luego lee el delimitador del heredoc como una serie de tokens de palabra.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Cadena de entrada que se está procesando
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
static void	handle_here_doc(t_token **tokens, char *input, int *i)
{
	int	quotes;

	quotes = 0;
	(*i) += 2;
	add_token(tokens, T_HERE_DOC, ft_strdup("<<"), 0);
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\"' || input[*i] == '\'')
	{
		quotes = 1;
		(*i)++;
	}
	while (input[*i] && (!ft_isspace(input[*i]) && input[*i] != '\"'
			&& input[*i] != '\''))
	{
		add_token(tokens, T_WORD, ft_substr(input, (*i), 1), quotes);
		(*i)++;
	}
}

/**
 * @brief Procesa operadores de redirección en la entrada
 *
 * Identifica y maneja los diferentes tipos de operadores de redirección:
 * - Redirección de entrada (<)
 * - Heredoc (<<)
 * - Redirección de salida (>)
 * - Redirección de salida con append (>>)
 *
 * Para cada tipo, crea los tokens correspondientes y avanza el
 * índice de lectura.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Cadena de entrada que se está procesando
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
void	handle_redirections(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
			handle_here_doc(tokens, input, i);
		else
		{
			(*i)++;
			add_token(tokens, T_REDIR_LEFT, ft_strdup("<"), 0);
		}
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, T_APPEND, ft_strdup(">>"), 0);
			(*i)++;
		}
		else
			add_token(tokens, T_REDIR_RIGHT, ft_strdup(">"), 0);
		(*i)++;
	}
}
