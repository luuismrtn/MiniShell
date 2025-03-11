/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:16:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 00:19:39 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_export(t_token *tokens, char **args)
{
	t_env	*current;
	t_env	*new_env;
	char	*var_name;
	char	*var_content;
	int		i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			var_name = ft_substr(args[i], 0, ft_strchr(args[i], '=') - args[i]);
			var_content = ft_strchr(args[i], '=') + 1;
			current = tokens->env_mshell;
			while (current)
			{
				if (ft_strncmp(current->name, var_name,
						ft_strlen(var_name)) == 0)
				{
					free(current->content);
					current->content = ft_strdup(var_content);
					break ;
				}
				current = current->next;
			}
			if (!current)
			{
				new_env = malloc(sizeof(t_env));
				if (!new_env)
					return ;
				new_env->name = ft_strdup(var_name);
				new_env->content = ft_strdup(var_content);
				new_env->next = tokens->env_mshell;
				tokens->env_mshell = new_env;
			}
			free(var_name);
		}
		i++;
	}
}
