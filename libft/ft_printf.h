/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:34:11 by lumartin          #+#    #+#             */
/*   Updated: 2024/10/05 22:28:49 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H

# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_putchar_fd_f(char c, int fd);
int	ft_putstr_fd_f(char *s, int fd);
int	ft_putnbr_fd_f(int n, int fd, int is_unsigned);
int	ft_puthex_fd_f(unsigned long n, int fd, int upper);
int	ft_putptr_fd_f(void *ptr, int fd);
int	ft_printf(const char *format, ...);

#endif