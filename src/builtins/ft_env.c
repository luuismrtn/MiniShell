/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/21 15:08:01 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Busca una variable de entorno por su nombre.
 *
 * Recorre la lista de variables de entorno comparando el nombre exacto
 * con el proporcionado. A diferencia de otras funciones similares, esta
 * devuelve un puntero a la estructura completa de la variable, no solo
 * su contenido.
 *
 * @param env Puntero a la lista enlazada de variables de entorno.
 * @param var_name Nombre de la variable a buscar.
 * @return t_env* Puntero a la estructura de la variable si se encuentra,
 *               NULL en caso contrario. No se debe liberar este puntero.
 */
t_env	*find_env_var(t_env *env, char *var_name)
{
	while (env)
	{
		if (match_string(env->name, var_name))
			return (env);
		env = env->next;
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
