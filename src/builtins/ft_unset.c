/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:16:46 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 00:19:43 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_unset(t_token *tokens, char **args)
{
	t_env	*current;
	t_env	*prev;
	int		i;

	i = 1;
	while (args[i])
	{
		current = tokens->env_mshell;
		prev = NULL;
		while (current)
		{
			if (ft_strncmp(current->name, args[i], ft_strlen(args[i])) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					tokens->env_mshell = current->next;
				free(current->name);
				free(current->content);
				free(current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
}
