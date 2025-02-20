/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:48:56 by lumartin          #+#    #+#             */
/*   Updated: 2024/09/25 16:49:13 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wordsize(int n, char const *s, char c)
{
	int	i;
	int	count;
	int	len;

	i = 0;
	len = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			count++;
			while (s[i] != c && s[i] != '\0')
			{
				if (n + 1 == count)
					len++;
				i++;
			}
		}
	}
	return (len);
}

static int	ft_wordcount(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
	}
	return (count);
}

static void	ft_free(char **split, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	**ft_savespace(char const *s, char c)
{
	int		i;
	int		size;
	char	**split;

	i = 0;
	size = ft_wordcount(s, c);
	split = malloc(sizeof(char *) * (size + 1));
	if (!split)
		return (NULL);
	while (i < size)
	{
		split[i] = malloc(sizeof(char) * (ft_wordsize(i, s, c) + 1));
		if (!split[i])
		{
			ft_free(split, i);
			return (NULL);
		}
		i++;
	}
	split[i] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	i = 0;
	k = 0;
	split = ft_savespace(s, c);
	if (!split)
		return (NULL);
	while (s[k] != '\0')
	{
		while (s[k] == c)
			k++;
		if (s[k] != c && s[k] != '\0')
		{
			j = 0;
			while (s[k] != c && s[k] != '\0')
				split[i][j++] = s[k++];
			split[i++][j] = '\0';
		}
	}
	return (split);
}
