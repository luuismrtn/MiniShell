/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:04:33 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/27 00:29:43 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Modifica o crea la variable SHLVL en el entorno
 *
 * Esta función se encarga de incrementar el nivel de shell (SHLVL)
 * cada vez que se inicia una nueva instancia de la shell. Si la variable
 * ya existe en el entorno, incrementa su valor en 1. Si no existe,
 * la crea con valor inicial 1.
 *
 * El nivel de shell (SHLVL) indica la profundidad de anidamiento de shells.
 * Por ejemplo, si ejecutas bash dentro de bash, SHLVL será 2.
 *
 * @param tokens Doble puntero al token que contiene la lista del entorno
 * @param var Nombre de la variable a modificar (normalmente "SHLVL")
 */
void	modify_shlvl(t_token **tokens, char *var)
{
	t_env	*aux;
	t_env	*new_var;
	int		shlvl;

	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (strncmp(aux->name, var, strlen(var)) == 0)
		{
			shlvl = ft_atoi(aux->content) + 1;
			free(aux->content);
			aux->content = ft_itoa(shlvl);
			return ;
		}
		aux = aux->next;
	}
	shlvl = 1;
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdup(var);
	new_var->content = ft_itoa(shlvl);
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}
