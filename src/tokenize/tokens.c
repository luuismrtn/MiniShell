/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:57:00 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/09 14:34:24 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Crea un nuevo nodo de token
 *
 * Asigna memoria para un nuevo token y lo inicializa con los valores
 * proporcionados. El contenido se duplica para evitar problemas de
 * gestión de memoria.
 *
 * @param type Tipo de token según la enumeración t_token_value
 * @param content Contenido del token (será duplicado)
 * @param quotes Indica si el token está entre comillas (1) o no (0)
 * @return t_token* Puntero al nuevo token creado, NULL si falla la asignación
 */
t_token	*create_node(t_token_value type, char *content, int quotes)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->content = ft_strdup(content);
	if (!new_token->content)
		return (free(new_token), NULL);
	new_token->quotes = quotes;
	new_token->env_mshell = NULL;
	new_token->next = NULL;
	return (new_token);
}

/**
 * @brief Añade un nuevo token al final de la lista
 *
 * Crea un nuevo nodo con los parámetros especificados y lo añade
 * al final de la lista enlazada. Si la lista está vacía, el nuevo
 * token se convierte en el primer elemento.
 *
 * @param head Doble puntero al inicio de la lista de tokens
 * @param type Tipo de token según la enumeración t_token_value
 * @param content Contenido del token (la función libera este puntero)
 * @param quotes Indica si el token está entre comillas (1) o no (0)
 */
void	add_token(t_token **head, t_token_value type, char *content, int quotes)
{
	t_token	*aux;
	t_token	*new_token;

	if (!content)
		return (free(content));
	new_token = create_node(type, content, quotes);
	if (!new_token || !head)
		return (free(content), free(new_token));
	if (*head == NULL)
		*head = new_token;
	else
	{
		aux = *head;
		while (aux->next != NULL)
			aux = aux->next;
		aux->next = new_token;
	}
	free(content);
}

/**
 * @brief Libera la memoria del primer token
 *
 * Esta función libera la memoria ocupada por el primer token
 * de la lista enlazada.
 *
 * @param tokens Puntero al primer token de la lista
 */
void	free_tokens_first(t_token *tokens)
{
	if (!tokens)
		return ;
	if (tokens->env_mshell)
		free_env_list(tokens->env_mshell);
	if (tokens->exp_var)
		free_env_list(tokens->exp_var);
	if (tokens->content)
		free(tokens->content);
	free(tokens);
}

/**
 * @brief Libera la memoria de todos los tokens
 *
 * Esta función libera la memoria ocupada por todos los tokens
 * de la lista enlazada.
 *
 * @param tokens Doble puntero al primer token de la lista
 */
void	free_tokens(t_token **tokens)
{
	t_token	*next;
	t_token	*aux;

	next = *tokens;
	while (next)
	{
		aux = next->next;
		free(next->content);
		free(next);
		next = aux;
	}
	*tokens = 0;
}

/**
 * @brief Duplica un token
 *
 * Esta función crea una copia de un token existente.
 * La copia incluye todos los campos del token original, excepto el contenido,
 * que se inicializa a NULL.
 * 
 * @param orig Token original a duplicar
 * @return Puntero al nuevo token duplicado
 */
t_token	*dup_token(t_token orig)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->env_mshell = orig.env_mshell;
	new->exp_var = orig.exp_var;
	new->type = orig.type;
	new->name_prog = orig.name_prog;
	new->content = 0;
	new->next = 0;
	return (new);
}
