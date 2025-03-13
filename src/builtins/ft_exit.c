/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:44 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 22:28:10 by lumartin         ###   ########.fr       */
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

static void	normalize_args(char ***arg)
{
	char	*new_arg;
	char	**args;
	int		i;

	args = *arg;
	if (args[0] && args[1] && (ft_strncmp(args[1], "-", 2) == 0
			|| ft_strncmp(args[1], "+", 2) == 0))
	{
		new_arg = ft_strjoin(args[1], args[2]);
		if (!new_arg)
			return ;
		free(args[1]);
		args[1] = new_arg;
		i = 2;
		while (args[i + 1])
		{
			args[i] = args[i + 1];
			i++;
		}
		args[i] = NULL;
	}
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

void	ft_exit(char **arg)
{
	int	args;

	normalize_args(&arg);
	args = count_args(arg);
	if (args > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit_num = 1;
	}
	else if (arg[1])
	{
		exit_num = ft_atoi(arg[1]);
		if (arg_isdigit(arg[1]))
			exit(exit_num);
		else
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(arg[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
	}
	else
		exit(0);
}
