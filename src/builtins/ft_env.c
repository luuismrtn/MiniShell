/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/14 17:20:34 by aldferna         ###   ########.fr       */
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

void get_env_content_and_replace(t_token **tokens, char *name, char *content)
{
	t_env *current;

	current = (*tokens)->env_mshell;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
		{
			free(current->content);
			current->content = content;
			break;
		}
		current = current->next;
	}
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
