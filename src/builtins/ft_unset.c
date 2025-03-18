/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:16:46 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/18 13:13:14 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Libera la memoria asociada a un nodo de variable de entorno.
 *
 * @param current Puntero al nodo de variable de entorno a liberar.
 */
static void	free_current(t_env *current)
{
	free(current->name);
	free(current->content);
	free(current);
}

/**
 * @brief Elimina una variable del entorno si existe.
 *
 * @param tokens Puntero a la estructura de tokens con la lista de variables.
 * @param var_name Nombre de la variable a eliminar.
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
 * @brief Implementa el comando unset.
 *
 * @param tokens Puntero a la estructura de tokens con la lista de variables.
 * @param args Array de argumentos del comando (args[0] es "unset").
 */
void	ft_unset(t_token *tokens, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(tokens, args[i]);
		i++;
	}
	exit_num = 0;
}
