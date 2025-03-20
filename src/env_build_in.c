/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_build_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:01:35 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/20 22:08:02 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Añade un nodo al final de la lista de variables de entorno.
 *
 * Esta función añade un nodo al final de la lista de variables de entorno.
 *
 * @param head Puntero al puntero de la cabeza de la lista.
 * @param node Nodo a añadir al final de la lista.
 */
void	add_last(t_env **head, t_env *node)
{
	t_env	*aux;

	if (*head == NULL)
		*head = node;
	else
	{
		aux = *head;
		while (aux->next != NULL)
			aux = aux->next;
		aux->next = node;
	}
}

/**
 * @brief Libera toda la memoria asignada para la lista de variables.
 *
 * Esta función libera toda la memoria asignada para la lista de variables de
 * entorno. Libera cada nodo y sus campos.
 *
 * @param env_list Puntero a la cabeza de la lista a liberar.
 */
void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->content);
		free(current);
		current = next;
	}
}

/**
 * @brief Crea un nuevo nodo de variable de entorno.
 *
 * Esta función crea un nuevo nodo de variable de entorno a partir de una cadena
 * de la forma "NAME=CONTENT". Si la cadena no contiene un signo igual ('='), se
 * considera un error y se devuelve NULL.
 *
 * @param env_var Cadena con la variable de entorno.
 * @return t_env* Puntero al nuevo nodo creado o NULL si hay un error.
 */
static t_env	*create_env_node(char *env_var)
{
	t_env	*node;
	char	*name;
	char	*content;
	int		x;

	x = 0;
	while (env_var[x] && env_var[x] != '=')
		x++;
	if (!env_var[x])
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	name = ft_substr(env_var, 0, x);
	if (!name)
		return (free(node), NULL);
	node->name = name;
	if (env_var[x] == '=')
		x++;
	content = ft_substr(env_var, x, ft_strlen(env_var) - x);
	if (!content)
		return (free(name), free(node), NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

/**
 * @brief Construye la lista de variables de entorno.
 *
 * Esta función construye la lista de variables de entorno a partir de un array
 * de cadenas con las variables de entorno. Cada cadena debe tener la forma
 * "NAME=CONTENT". Si alguna cadena no tiene el formato correcto, se considera
 * un error y se devuelve NULL.
 *
 * @param env Array de cadenas con las variables de entorno.
 * @return t_env* Puntero a la cabeza de la nueva lista.
 */
t_env	*env_buildin(char **env)
{
	t_env	*env_mshell;
	t_env	*node;
	int		i;

	env_mshell = NULL;
	i = 0;
	while (env[i])
	{
		node = create_env_node(env[i]);
		if (!node && env[i][0] != '\0')
		{
			free_env_list(env_mshell);
			return (NULL);
		}
		if (node)
			add_last(&env_mshell, node);
		i++;
	}
	return (env_mshell);
}
