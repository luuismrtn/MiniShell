/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:58:28 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/25 15:24:42 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	num_pipes(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count++;
		i++;
	}
	return (count);
}

int	has_pipe(t_token *tokens)
{
	t_token	*aux;

	aux = tokens->next;
	while (aux != NULL)
	{
		if (aux->type == T_PIPE)
			return (1);
		aux = aux->next;
	}
	return (0);
}

