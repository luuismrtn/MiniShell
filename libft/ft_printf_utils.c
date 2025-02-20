/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:02:14 by lumartin          #+#    #+#             */
/*   Updated: 2024/10/05 22:37:45 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_fd_f(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putnbr_fd_f(int n, int fd, int is_unsigned)
{
	int				count;
	unsigned int	un;

	count = 0;
	if (!is_unsigned && n == -2147483648)
		return (count += ft_putstr_fd_f("-2147483648", fd));
	if (!is_unsigned && n < 0)
	{
		count += ft_putchar_fd_f('-', fd);
		n = -n;
	}
	if (is_unsigned)
	{
		un = (unsigned int)n;
		if (un > 9)
			count += ft_putnbr_fd_f(un / 10, fd, 1);
		count += ft_putchar_fd_f(un % 10 + '0', fd);
	}
	else
	{
		if (n > 9)
			count += ft_putnbr_fd_f(n / 10, fd, 0);
		count += ft_putchar_fd_f(n % 10 + '0', fd);
	}
	return (count);
}

int	ft_putstr_fd_f(char *s, int fd)
{
	int	i;
	int	count;

	if (!s)
		return (ft_putstr_fd_f("(null)", fd));
	i = 0;
	count = 0;
	while (s[i])
		count += ft_putchar_fd_f(s[i++], fd);
	return (count);
}

int	ft_puthex_fd_f(unsigned long n, int fd, int upper)
{
	char	*base;
	int		count;

	count = 0;
	if (upper == 0)
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (n >= 16)
		count += ft_puthex_fd_f(n / 16, fd, upper);
	count += ft_putchar_fd_f(base[n % 16], fd);
	return (count);
}

int	ft_putptr_fd_f(void *ptr, int fd)
{
	int				count;
	unsigned long	ptr_value;

	if (!ptr)
		return (ft_putstr_fd_f("(nil)", fd));
	count = 0;
	ptr_value = (unsigned long)ptr;
	count += ft_putstr_fd_f("0x", fd);
	count += ft_puthex_fd_f(ptr_value, fd, 0);
	return (count);
}
