/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:16 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/21 17:37:00 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Imprime los argumentos del comando echo.
 *
 * Muestra todos los argumentos del comando echo separados por espacios.
 * Si la opción -n no está activada, añade un salto de línea al final.
 *
 * @param args Array de argumentos del comando.
 * @param n Flag que indica si se debe omitir el salto de línea final
 * (1 = omitir).
 * @param i Índice inicial desde donde empezar a imprimir los argumentos.
 */
static void	print_echo(char **args, int n, int i)
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

/**
 * @brief Implementa el comando echo.
 *
 * Muestra los argumentos proporcionados en la salida estándar.
 * Si se especifica la opción -n, no añade un salto de línea al final.
 * Sin argumentos, simplemente muestra un salto de línea.
 *
 * Comportamiento:
 * - "echo" → Muestra solo un salto de línea
 * - "echo texto" → Muestra "texto" seguido de un salto de línea
 * - "echo -n texto" → Muestra "texto" sin salto de línea
 * - "echo arg1 arg2 ..." → Muestra los argumentos separados por espacios
 *
 * @param args Array de argumentos del comando (args[0] es "echo").
 */
void	ft_echo(char **args)
{
	int	i;
	int	n;

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
	exit_num = 0;
}
