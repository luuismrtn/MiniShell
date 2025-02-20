/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:23:18 by lumartin          #+#    #+#             */
/*   Updated: 2024/09/25 15:44:58 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	num_len(int n)
{
	long int	num;
	size_t		i;

	num = n;
	i = 1;
	if (num < 0)
		num *= -1;
	while (num >= 10)
	{
		num /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*str_num;
	size_t		len;
	long int	num;

	num = n;
	len = num_len(n);
	if (n < 0)
	{
		num *= -1;
		len++;
	}
	str_num = malloc(sizeof(char) * (len + 1));
	if (!str_num)
		return (0);
	str_num[len] = 0;
	while (len--)
	{
		str_num[len] = num % 10 + '0';
		num = num / 10;
	}
	if (n < 0)
		str_num[0] = '-';
	return (str_num);
}
