/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 18:06:14 by lumartin          #+#    #+#             */
/*   Updated: 2024/10/05 22:29:01 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	ft_check_char(char c)
{
	if (c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i' || c == 'u'
		|| c == 'x' || c == 'X' || c == '%')
		return (c);
	return (0);
}

static int	ft_print(char c, va_list args)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar_fd_f(va_arg(args, int), 1);
	else if (c == 's')
		count += ft_putstr_fd_f(va_arg(args, char *), 1);
	else if (c == 'p')
		count += ft_putptr_fd_f(va_arg(args, void *), 1);
	else if (c == 'd' || c == 'i')
		count += ft_putnbr_fd_f(va_arg(args, int), 1, 0);
	else if (c == 'u')
		count += ft_putnbr_fd_f(va_arg(args, unsigned int), 1, 1);
	else if (c == 'x')
		count += ft_puthex_fd_f(va_arg(args, unsigned int), 1, 0);
	else if (c == 'X')
		count += ft_puthex_fd_f(va_arg(args, unsigned int), 1, 1);
	else if (c == '%')
		count += ft_putchar_fd_f('%', 1);
	return (count);
}

int	ft_printf(char const *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	va_start(args, format);
	i = 0;
	count = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%' && ft_check_char(format[i + 1]) != 0)
		{
			i++;
			count += ft_print(format[i], args);
		}
		else
			count += ft_putchar_fd_f(format[i], 1);
		i++;
	}
	va_end(args);
	return (count);
}
