/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 21:07:16 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *get_env_content(t_env *env, char *name)
{
	t_env *current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
			return (current->content);
		current = current->next;
	}
	return (NULL);
}

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
