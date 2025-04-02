/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:07:26 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/02 20:01:09 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Expande una variable de entorno dentro de comillas
 *
 * Busca una variable de entorno en la lista de entorno y sustituye
 * su nombre por su contenido. Gestiona la memoria liberando la cadena
 * de resultado anterior y el nombre de la variable.
 *
 * @param result Cadena acumulada hasta el momento (se libera en la función)
 * @param var_name Nombre de la variable a expandir (se libera en la función)
 * @param tokens Doble puntero a la lista de tokens para acceso al entorno
 * @return char* Nueva cadena con la variable expandida
 */
char	*expand_var_in_quotes(char *result, char *var_name, t_token **tokens)
{
	t_env	*temp_var;
	char	*temp;
	char	*var_content;

	temp_var = find_env_var((*tokens)->env_mshell, var_name);
	if (!temp_var)
	{
		temp = ft_strjoin(result, "");
		free(result);
		free(var_name);
		return (temp);
	}
	var_content = temp_var->content;
	temp = ft_strjoin(result, var_content);
	free(result);
	free(var_name);
	return (temp);
}

/**
 * @brief Procesa variables de entorno dentro de comillas dobles
 *
 * Identifica y expande variables de entorno (símbolos $ seguidos de un nombre)
 * cuando se encuentran dentro de comillas dobles. Maneja casos especiales
 * como $? para el código de salida del último comando.
 *
 * @param result Cadena acumulada hasta el momento (se libera en la función)
 * @param input Cadena de entrada completa
 * @param i Puntero al índice actual de lectura (se actualiza en la función)
 * @param tokens Doble puntero a la lista de tokens para acceso al entorno
 * @return char* Nueva cadena con la variable expandida
 */
char	*process_env_in_quotes(char *result, char *input, int *i,
		t_token **tokens)
{
	char	*temp;
	char	*value;
	int		var_start;
	int		var_len;

	(*i)++;
	if (input[*i] == '?')
	{
		value = ft_itoa(g_exit_num);
		temp = ft_strjoin(result, value);
		(*i)++;
		return (free(result), free(value), temp);
	}
	var_start = *i;
	var_len = ft_len_var_name(input, *i);
	*i += var_len;
	if (var_len > 0)
		return (expand_var_in_quotes(result, ft_substr(input, var_start,
					var_len), tokens));
	else
	{
		temp = ft_strjoin(result, "$");
		free(result);
		return (temp);
	}
}
