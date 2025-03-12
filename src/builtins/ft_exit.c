/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:44 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 14:31:20 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	arg_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i == 0 && (str[i] == '-' || str[i] == '+') && str[i + 1])
			i++;
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	if (i > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		exit_num = 1;
	}
	else if (arg[1])
	{
		exit_num = ft_atoi(arg[1]);
		if (arg_isdigit(arg[1]))
			exit(exit_num);
		else
		{
			write(2, "numeric argument required\n", 27);
			exit(2);
		}
	}
	else
		exit(0);
}
