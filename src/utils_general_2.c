/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:49:19 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/08 23:54:46 by lumartin         ###   ########.fr       */
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

/**
 * @brief Reemplaza el último comando ejecutado en la variable de entorno "_"
 * 
 * Esta función busca la variable de entorno "_" y la reemplaza con el
 * último comando ejecutado.
 * 
 * @param tokens Token que contiene la lista de variables de entorno
 */
void	replace_last_command(t_token *tokens)
{
	t_env	*slash;
	t_token	*last_command;
	char	*last_word;

	last_command = tokens;
	while (last_command->next)
		last_command = last_command->next;
	last_word = last_command->content;
	slash = find_env_var(tokens->env_mshell, "_");
	if (slash)
	{
		free(slash->content);
		slash->content = ft_strdup(last_word);
	}
}
