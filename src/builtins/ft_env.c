/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 00:19:34 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd(current->name, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(current->content, 1);
		current = current->next;
	}
}
