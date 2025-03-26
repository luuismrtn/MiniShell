/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:57:00 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/27 00:12:01 by lumartin         ###   ########.fr       */
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
		return ;
	new_token = create_node(type, content, quotes);
	if (!new_token || !head)
		return ;
	if (*head == NULL)
		*head = new_token;
	else
	{
		aux = *head;
		while (aux->next != NULL)
			aux = aux->next;
		aux->next = new_token;
	}
}

/**
 * @brief Elimina todos los tokens de la lista y reinicializa el nodo inicial
 *
 * Esta función libera la memoria de todos los tokens en la lista, pero
 * conserva la información de entorno (env_mshell),	variables de expansión
 * (exp_var) y el contenido del nodo inicial. Luego crea un nuevo nodo
 * inicial con estos valores preservados.
 *
 * @param tokens Doble puntero al inicio de la lista de tokens
 */
void	delete_tokens(t_token **tokens)
{
	t_env	*env;
	t_env	*var;
	char	*content;
	t_token	*aux;
	t_token	*next;

	env = NULL;
	var = (*tokens)->exp_var;
	env = (*tokens)->env_mshell;
	content = (*tokens)->content;
	aux = *tokens;
	while (aux != NULL)
	{
		next = aux->next;
		if (aux->content)
			aux->content = NULL;
		free(aux);
		aux = next;
	}
	*tokens = malloc(sizeof(t_token));
	ft_memset(*tokens, 0, sizeof(t_token));
	(*tokens)->env_mshell = env;
	(*tokens)->exp_var = var;
	(*tokens)->content = content;
}
