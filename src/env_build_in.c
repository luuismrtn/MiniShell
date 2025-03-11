/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_build_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:01:35 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/12 00:40:29 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	add_last(t_env **head, t_env *node)
{
	t_env	*aux;

	if (*head == NULL)
		*head = node;
	else
	{
		aux = *head;
		while (aux->next != NULL)
			aux = aux->next;
		aux->next = node;
	}
}

t_env	*env_buildin(char **env)
{
	t_env	*env_mshell;
	t_env	*node;
	char	*name;
	char	*content;
	int		i;
	int		x;

	env_mshell = NULL;
	i = 0;
	while (env[i])
	{
		x = 0;
		while (env[i][x] && env[i][x] != '=')
			x++;
		if (!env[i][x])
			return (env_mshell);
		node = malloc(sizeof(t_env));
		if (!node)
			return (NULL);
		name = ft_substr(env[i], 0, x);
		if (!name)
			return (free(node), NULL);
		node->name = name;
		if (env[i][x] == '=')
			x++;
		content = ft_substr(env[i], x, ft_strlen(env[i]) - x);
		if (!content)
			return (free(node->name), free(node), NULL);
		node->content = content;
		node->next = NULL;
		add_last(&env_mshell, node);
		i++;
	}
	return (env_mshell);
}
