/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/17 17:11:28 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void print_echo(char **args, int n, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n)
		ft_putchar_fd('\n', 1);
}

void	ft_echo(char **args)
{
	int	i;
	int	n;

	//print_2(args);
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	if (ft_strncmp(args[1], "-n", 3) == 0)
	{
		n = 1;
		i = 2;
	}
	else
	{
		n = 0;
		i = 1;
	}
	print_echo(args, n, i);
}
