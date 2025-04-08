/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:53:45 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/08 22:10:24 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Procesa espacios en blanco en la cadena de entrada
 *
 * Identifica secuencias de espacios en blanco y crea un único token
 * que los representa. Avanza el índice de lectura hasta el primer
 * carácter que no sea un espacio.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Cadena de entrada que se está procesando
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
static void	handle_spaces(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = *i;
	while (ft_isspace(input[*i]))
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, T_SPACE, content, 0);
}

/**
 * @brief Procesa caracteres de tubería (pipe) en la cadena de entrada
 *
 * Crea un token para representar el operador de tubería ('|') y
 * avanza el índice de lectura.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Carácter actual de la entrada (debe ser '|')
 * @param type Tipo de token a crear (T_PIPE)
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
static void	handle_pipe(t_token **tokens, char input, t_token_value type,
		int *i)
{
	char	quotes[2];

	quotes[0] = input;
	quotes[1] = '\0';
	(*i)++;
	add_token(tokens, type, ft_strdup(quotes), 0);
}

/**
 * @brief Procesa variables de entorno en la cadena de entrada
 *
 * Identifica y expande variables de entorno (precedidas por '$').
 * Maneja casos especiales como '$?' para el código de salida y
 * concatenación de múltiples variables.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Cadena de entrada que se está procesando
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
static void	handle_env(t_token **tokens, char *input, int *i)
{
	int		len_var_name;
	char	*var_name;
	char	*var_content;

	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		return (add_token(tokens, T_ENV, ft_itoa(g_exit_num), 0));
	}
	len_var_name = ft_len_var_name(input, *i);
	if (len_var_name == 0)
		return (add_token(tokens, T_WORD, ft_strdup(""), 0));
	var_name = ft_substr(input, *i, len_var_name);
	(*i) += len_var_name;
	if (!check_var_exist(var_name, *tokens))
		return (free(var_name));
	var_content = ft_strdup(find_env_var((*tokens)->env_mshell,
				var_name)->content);
	add_token(tokens, T_ENV, var_content, 0);
	free(var_name);
	if (input[*i] == '$' && input[*i + 1] != '\0' && (ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '_' || input[*i + 1] == '?'))
		handle_env(tokens, input, i);
}

/**
 * @brief Procesa palabras y flags en la cadena de entrada
 *
 * Identifica secuencias de caracteres que forman palabras o flags
 * (que comienzan con '-'). Maneja casos especiales como caracteres
 * escapados con '\\'.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param input Cadena de entrada que se está procesando
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
static void	handle_word(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = *i;
	if (input[*i] == '\\')
	{
		(*i)++;
		start++;
	}
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\"'
		&& input[*i] != '\'' && input[*i] != '|' && input[*i] != '<'
		&& input[*i] != '>' && !(input[*i] == '$' && input[*i + 1] != '\0'))
		(*i)++;
	content = ft_substr(input, start, *i - start);
	if (!content)
		return ;
	if (content[0] == '-')
		add_token(tokens, T_FLAG, content, 0);
	else
		add_token(tokens, T_WORD, content, 0);
}

/**
 * @brief Divide la entrada en tokens según las reglas de shell
 *
 * Función principal de tokenización que procesa una cadena de entrada
 * y la convierte en una serie de tokens estructurados. Identifica y
 * maneja diferentes elementos como espacios, comillas, redirecciones,
 * tuberías, variables de entorno y palabras.
 *
 * @param input Cadena de entrada a tokenizar
 * @param tokens Token inicial (cabeza de la lista) donde comenzar a añadir
 * @return t_token* Lista de tokens creada a partir de la entrada
 */
t_token	*tokenize(char *input, t_token *tokens)
{
	int	i;

	if (!input)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			handle_spaces(&tokens, input, &i);
		else if (input[i] == '\'')
			handle_quotes(&tokens, input, &i, T_S_QUOTE);
		else if (input[i] == '\"')
			handle_quotes(&tokens, input, &i, T_D_QUOTE);
		else if (input[i] == '<' || input[i] == '>')
			handle_redirections(&tokens, input, &i);
		else if (input[i] == '|')
			handle_pipe(&tokens, input[i], T_PIPE, &i);
		else if (input[i] == '$' && input[i + 1] && input[i + 1] != '\0')
			handle_env(&tokens, input, &i);
		else if (input[i] == '~')
			handle_vir(&tokens, input, &i);
		else
			handle_word(&tokens, input, &i);
	}
	return (tokens);
}
