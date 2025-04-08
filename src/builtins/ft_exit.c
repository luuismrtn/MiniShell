/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:17:44 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/08 23:05:34 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Verifica si una cadena representa un número válido.
 *
 * Comprueba si todos los caracteres de la cadena son dígitos,
 * permitiendo un signo (+ o -) únicamente al principio.
 *
 * @param str La cadena a verificar.
 * @return int 1 si la cadena es un número válido, 0 en caso contrario.
 */
static int	arg_isdigit(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
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

/**
 * @brief Normaliza los argumentos para el comando exit.
 *
 * Procesa casos especiales como cuando un signo (+ o -) se proporciona
 * como un argumento separado antes de un número. En este caso, une el
 * signo con el número siguiente y reorganiza el resto de argumentos.
 *
 * Ejemplo: ["exit", "-", "123"] se convierte en ["exit", "-123"]
 *
 * @param arg Dirección de memoria de los argumentos del comando.
 */
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

/**
 * @brief Implementa el comando exit.
 *
 * Termina la ejecución del shell con el código de salida especificado.
 * Si no se proporciona un argumento, termina con código 0.
 * Si se proporciona un argumento numérico, se usa como código de salida.
 * Si se proporcionan demasiados argumentos, muestra un error y no termina.
 * Si el argumento no es numérico, muestra un error y termina con código 2.
 *
 * @param arg Array de argumentos del comando (arg[0] es "exit").
 */
void	ft_exit(char **arg)
{
	int	args;

	normalize_args(&arg);
	args = len_array(arg);
	ft_putstr_fd("exit\n", 2);
	if (args > 2 && ft_isdigit(arg[1][0]))
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_exit_num = 1;
	}
	else if (arg[1])
	{
		g_exit_num = ft_atoi(arg[1]);
		if (arg_isdigit(arg[1]))
			exit(g_exit_num);
		else
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
	}
	else
		exit(1);
}
