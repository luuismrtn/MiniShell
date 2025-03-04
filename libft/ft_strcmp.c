/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 14:36:05 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/04 14:36:24 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strcmp(char *str1, char *str2)
{
	int i;

	i = 0;
	if (!str1 || !str2)
		return 1;
	while(str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i])
			return 1;
		i++;
	}
	return 0;
}