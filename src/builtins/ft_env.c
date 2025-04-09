/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/09 15:01:47 by lumartin         ###   ########.fr       */
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
 * @brief Inicializa la estructura principal del shell.
 *
 * Asigna memoria para la estructura principal del shell y la inicializa
 * con valores predeterminados. Crea una lista de variables de entorno
 * a partir del array proporcionado.
 *
 * @param env Array de strings con las variables de entorno.
 * @return t_token* Puntero a la estructura principal del shell, NULL si falla
 */
t_token	*initialize_shell(char **env, char **vars, char *argv)
{
	t_token	*tokens;

	tokens = malloc(sizeof(t_token));
	if (!tokens)
		return (NULL);
	ft_memset(tokens, 0, sizeof(t_token));
	if (!env || !env[0])
		tokens->env_mshell = make_little_env();
	else
		tokens->env_mshell = env_buildin(env);
	if (vars)
		tokens->exp_var = env_buildin(vars);
	else
		tokens->exp_var = NULL;
	tokens->name_prog = argv;
	tokens->content = NULL;
	signals('f');
	return (tokens);
}

/**
 * @brief Implementa el comando env.
 *
 * Muestra todas las variables de entorno definidas en el formato
 * "nombre=valor",
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
