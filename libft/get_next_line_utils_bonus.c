/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 19:03:43 by lumartin          #+#    #+#             */
/*   Updated: 2024/10/05 22:31:51 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strjoin_gnl(char *s1, const char *s2, int len)
{
	char	*str;

	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	str = ft_fill_join(s1, s2, len);
	free(s1);
	return (str);
}

static char	*ft_free(char *str)
{
	if (str)
		free(str);
	return (NULL);
}

char	*ft_fill_join(char *s1, const char *s2, int len)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * (ft_strlen(s1) + len + 1));
	if (!str)
		return (ft_free(s1));
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (j < len)
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}
