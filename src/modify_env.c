/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:04:33 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/25 15:04:48 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

