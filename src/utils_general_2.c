/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:49:19 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/03 21:56:09 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Verifica si una variable de entorno existe
 *
 * Esta función busca una variable de entorno por su nombre en la lista
 * de variables almacenadas en la estructura tokens.
 *
 * @param var Nombre de la variable a buscar
 * @param tokens Token que contiene la lista de variables de entorno
 * @return int 1 si la variable existe, 0 si no existe
 */
int	check_var_exist(char *var, t_token *tokens)
{
	t_env	*current;

	current = tokens->env_mshell;
	while (current)
	{
		if (match_string(current->name, var))
			return (1);
		current = current->next;
	}
	return (0);
}
