/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:16:46 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/04 15:52:55 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Libera la memoria asociada a un nodo de variable de entorno
 *
 * Esta función se encarga de liberar toda la memoria asignada
 * para un nodo de la lista de variables de entorno, incluyendo
 * el nombre, el contenido y la estructura del nodo.
 *
 * @param current Puntero al nodo que se va a liberar
 */
static void	free_current(t_env *current)
{
	free(current->name);
	free(current->content);
	free(current);
}

/**
 * @brief Elimina una variable de entorno específica de la lista
 *
 * Esta función busca en la lista de variables de entorno
 * una variable que coincida con el nombre proporcionado y la elimina.
 * Maneja correctamente el caso especial cuando la variable a eliminar
 * es la primera de la lista, actualizando el puntero principal.
 *
 * @param tokens Token que contiene la lista de variables de entorno
 * @param var_name Nombre de la variable a eliminar
 */
static void	remove_env_var(t_token *tokens, char *var_name)
{
	t_env	*current;
	t_env	*prev;

	current = tokens->env_mshell;
	prev = NULL;
	while (current)
	{
		if (match_string(current->name, var_name))
		{
			if (prev)
				prev->next = current->next;
			else
				tokens->env_mshell = current->next;
			free_current(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * @brief Elimina una variable exportada de la lista
 *
 * Esta función busca en la lista de variables exportadas
 * una variable que coincida con el nombre proporcionado y la elimina.
 * Maneja correctamente el caso especial cuando la variable a eliminar
 * es la primera de la lista, actualizando el puntero principal.
 *
 * @param tokens Token que contiene la lista de variables exportadas
 * @param var_name Nombre de la variable a eliminar
 */
static void	remove_export_var(t_token *tokens, char *var_name)
{
	t_env	*current;
	t_env	*prev;

	current = tokens->exp_var;
	prev = NULL;
	while (current)
	{
		if (match_string(current->name, var_name))
		{
			if (prev)
				prev->next = current->next;
			else
				tokens->exp_var = current->next;
			free_current(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * @brief Implementa el comando builtin 'unset'
 *
 * El comando unset elimina una o más variables de entorno.
 * Procesa cada argumento proporcionado como nombre de variable a eliminar.
 * Si una variable no existe, la función simplemente continúa con la siguiente.
 * Al finalizar, establece el código de salida a 0 para indicar éxito.
 *
 * Sintaxis: unset VARIABLE1 [VARIABLE2 ...]
 *
 * @param tokens Token que contiene la lista de variables de entorno
 * @param args Array de argumentos, donde args[0] es "unset" y
 *             los siguientes son los nombres de variables a eliminar
 */
void	ft_unset(t_token *tokens, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (find_env_var(tokens->env_mshell, args[i]))
			remove_env_var(tokens, args[i]);
		else
			remove_export_var(tokens, args[i]);
		i++;
	}
	g_exit_num = 0;
}
