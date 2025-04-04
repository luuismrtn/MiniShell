/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:48:33 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/04 18:26:47 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Determina si dos tokens deben fusionarse
 *
 * Verifica si dos tokens consecutivos deben combinarse en uno solo
 * basándose en sus tipos. Los tokens se fusionan si son del mismo tipo o
 * si son combinaciones específicas de palabras y variables de entorno.
 *
 * @param current El token actual
 * @param next El siguiente token
 * @return int 1 si los tokens deben fusionarse, 0 si no
 */
static int	should_merge_tokens(t_token *current, t_token *next)
{
	if ((current->type == T_PIPE && next->type == T_PIPE)
		&& (match_string(current->content, next->content)))
		return (0);
	if (current->type == next->type)
		return (1);
	if (current->type == T_WORD && next->type == T_ENV)
		return (1);
	if (current->type == T_ENV && next->type == T_WORD)
		return (1);
	if (current->type == T_FLAG && next->type == T_WORD && next->quotes == 1)
		return (1);
	return (0);
}

/**
 * @brief Fusiona dos tokens consecutivos
 *
 * Combina el contenido del token actual con el siguiente,
 * actualiza las propiedades del token actual y elimina el token siguiente.
 *
 * @param current El token actual que mantendrá el contenido combinado
 * @return t_token* El token actual después de la fusión
 */
static t_token	*merge_tokens(t_token *current)
{
	t_token	*temp;
	char	*old_content;
	char	*new_content;

	old_content = current->content;
	new_content = ft_strjoin(old_content, current->next->content);
	if (!new_content)
		return (current->next);
	free(old_content);
	current->content = ft_strdup(new_content);
	free(new_content);
	if (current->quotes == 0 && current->next->quotes == 1)
		current->quotes = 1;
	temp = current->next;
	current->next = current->next->next;
	free(temp->content);
	free(temp);
	return (current);
}

/**
 * @brief Procesa un token de espacio
 *
 * Cuando se encuentra un token de espacio, lo convierte en un token
 * del mismo tipo que el siguiente token, manteniendo la cadena vacía
 * o un espacio como contenido.
 *
 * @param current El token actual (espacio) que será modificado
 * @return t_token* El siguiente token a procesar
 */
static t_token	*handle_space_token(t_token *current)
{
	if (current->next->next != NULL)
	{
		current = current->next;
		free(current->content);
		current->content = ft_strdup("");
		if (!current->content)
			current->content = ft_strdup(" ");
		current->type = current->next->type;
		return (current);
	}
	else
		return (current->next);
}

/**
 * @brief Limpia el último token de la lista
 *
 * Elimina el último token de la lista si es un espacio.
 *
 * @param tokens Puntero a la lista de tokens
 */
static void	clean_last_token(t_token **tokens)
{
	t_token	*aux;
	t_token	*last;

	if (!tokens || !*tokens)
		return ;
	aux = *tokens;
	while (aux->next != NULL)
	{
		last = aux;
		aux = aux->next;
	}
	if (aux->type == T_SPACE)
	{
		free(aux->content);
		free(aux);
		last->next = NULL;
	}
}

/**
 * @brief Limpia y optimiza la lista de tokens
 *
 * Esta función recorre la lista de tokens, fusionando tokens consecutivos
 * del mismo tipo o con tipos compatibles y procesando los espacios.
 * La optimización ayuda a simplificar la lista para el análisis posterior.
 *
 * @param tokens Puntero a la lista de tokens a limpiar
 */
void	clean_tokens(t_token **tokens)
{
	t_token	*aux;

	if (!tokens || !*tokens)
		return ;
	aux = *tokens;
	aux = aux->next;
	while (aux != NULL && aux->next != NULL)
	{
		if (aux->type != T_SPACE && aux->next->type != T_SPACE)
		{
			if (should_merge_tokens(aux, aux->next))
				aux = merge_tokens(aux);
			else
				aux = aux->next;
		}
		else
		{
			aux = handle_space_token(aux);
		}
	}
	clean_last_token(tokens);
}
