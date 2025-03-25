/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:07:26 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/25 21:44:24 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Procesa un carácter dentro de comillas
 *
 * Añade un carácter al resultado acumulado cuando se está procesando
 * texto dentro de comillas. Maneja la gestión de memoria liberando
 * el resultado anterior y avanzando el índice.
 *
 * @param result Cadena acumulada hasta el momento (se libera dentro de
 * la función)
 * @param input Cadena de entrada completa
 * @param i Puntero al índice actual de lectura
 * @return char* Nueva cadena con el carácter añadido
 */
static char	*process_quoted_char(char *result, char *input, int *i)
{
	char	c[2];
	char	*temp;

	c[0] = input[*i];
	c[1] = '\0';
	temp = ft_strjoin(result, c);
	free(result);
	(*i)++;
	return (temp);
}

/**
 * @brief Procesa el contenido dentro de comillas dobles
 *
 * Extrae y procesa el texto entre comillas dobles, manejando caracteres
 * especiales y expansión de variables. Las comillas dobles permiten
 * la expansión de variables con $.
 *
 * @param input Cadena de entrada completa
 * @param i Índice donde comienza el contenido entre comillas
 * @param tokens Doble puntero a la lista de tokens para acceder al entorno
 * @return t_result Estructura con el contenido extraído y su longitud
 */
static t_result	process_double_quotes(char *input, int i, t_token **tokens)
{
	t_result	data;
	char		*result;
	int			start;
	int			j;

	ft_memset(&data, 0, sizeof(t_result));
	start = i;
	j = 0;
	while (input[i] && !(input[i] == '\"' && (i == 0 || input[i - 1] != '\\')))
		i++;
	j = i - start;
	i = start;
	result = malloc((j + 1) * sizeof(char));
	result[0] = '\0';
	while (input[i] && !(input[i] == '\"' && (i == 0 || input[i - 1] != '\\')))
	{
		if (input[i] == '$' && input[i + 1] && input[i + 1] != '\"' && input[i
				+ 1] != ' ')
			result = process_env_in_quotes(result, input, &i, tokens);
		else
			result = process_quoted_char(result, input, &i);
	}
	data.content = result;
	data.len = i - start;
	return (data);
}

/**
 * @brief Procesa el contenido dentro de comillas simples
 *
 * Extrae el texto entre comillas simples tal cual, sin realizar
 * ningún tipo de expansión o interpretación de caracteres especiales.
 * Las comillas simples preservan el texto literal.
 *
 * @param input Cadena de entrada completa
 * @param i Índice donde comienza el contenido entre comillas
 * @return t_result Estructura con el contenido extraído y su longitud
 */
static t_result	process_single_quotes(char *input, int i)
{
	t_result	data;
	int			start;
	int			j;

	ft_memset(&data, 0, sizeof(t_result));
	start = i;
	j = 0;
	while (input[i] && !(input[i] == '\'' && (i == 0 || input[i - 1] != '\\')))
	{
		j++;
		i++;
	}
	data.content = ft_substr(input, start, j);
	data.len = j;
	return (data);
}

/**
 * @brief Extrae el contenido entre comillas según su tipo
 *
 * Función de despacho que llama al procesador adecuado según
 * el tipo de comillas (simples o dobles) para extraer su contenido.
 *
 * @param type Tipo de comillas (T_S_QUOTE o T_D_QUOTE)
 * @param input Cadena de entrada completa
 * @param i Índice donde comienza el contenido entre comillas
 * @param tokens Doble puntero a la lista de tokens (usado para comillas dobles)
 * @return t_result Estructura con el contenido extraído y su longitud
 */
t_result	content_in_quotes(t_token_value type, char *input, int i,
		t_token **tokens)
{
	t_result	data;

	if (type == T_D_QUOTE)
		return (process_double_quotes(input, i, tokens));
	else if (type == T_S_QUOTE)
		return (process_single_quotes(input, i));
	ft_memset(&data, 0, sizeof(t_result));
	return (data);
}

/**
 * @brief Maneja el procesamiento de comillas en la entrada
 *
 * Función principal para gestionar comillas en el proceso de tokenización.
 * Identifica el tipo de comillas, extrae su contenido y crea los tokens
 * correspondientes. Avanza adecuadamente los índices en la cadena de entrada.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Cadena de entrada completa
 * @param i Puntero al índice actual de lectura (se actualiza en la función)
 * @param type Tipo de comillas encontradas (T_S_QUOTE o T_D_QUOTE)
 */
void	handle_quotes(t_token **tokens, char *input, int *i, t_token_value type)
{
	t_result	data;

	(*i)++;
	if (!input[*i])
	{
		add_token(tokens, T_WORD, ft_strdup(""), 1);
		return ;
	}
	data = content_in_quotes(type, input, *i, tokens);
	if (data.content)
	{
		add_token(tokens, T_WORD, data.content, 1);
		(*i) += data.len;
		if ((type == T_D_QUOTE && input[*i] == '\"') || (type == T_S_QUOTE
				&& input[*i] == '\''))
			(*i)++;
	}
	else
		add_token(tokens, T_WORD, ft_strdup(""), 1);
}
