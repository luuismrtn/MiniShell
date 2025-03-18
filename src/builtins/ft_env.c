/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/18 12:24:13 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Obtiene el contenido de una variable de entorno.
 *
 * Busca una variable por su nombre en la lista enlazada de variables
 * de entorno y devuelve su contenido si la encuentra.
 *
 * @param env Puntero a la lista enlazada de variables de entorno.
 * @param name Nombre de la variable a buscar.
 * @return char* Contenido de la variable si existe, NULL en caso contrario.
 *               No liberar este puntero, es una referencia directa.
 */
char	*get_env_content(t_env *env, char *name)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
			return (current->content);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Actualiza el contenido de una variable de entorno existente.
 *
 * Busca una variable por su nombre y reemplaza su contenido por el nuevo valor
 * proporcionado. Libera la memoria del contenido anterior.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param name Nombre de la variable a actualizar.
 * @param content Nuevo contenido para la variable. Este puntero será propiedad
 *                de la estructura de entorno después de la llamada.
 */
void	get_env_content_and_replace(t_token **tokens, char *name, char *content)
{
	t_env	*current;

	current = (*tokens)->env_mshell;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
		{
			free(current->content);
			current->content = content;
			break ;
		}
		current = current->next;
	}
}

/**
 * @brief Implementa el comando env.
 *
 * Muestra todas las variables de entorno definidas en el formato "nombre=valor",
 * una por línea. Solo muestra variables que tienen valor asignado
 * (content no nulo).
 *
 * @param env Puntero a la lista enlazada de variables de entorno.
 */
void	ft_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->content)
			printf("%s=%s\n", current->name, current->content);
		current = current->next;
	}
}
