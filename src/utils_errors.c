/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:19:51 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/26 20:20:24 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void check_quotes(char *l, int i, int (*in_quote_s_d)[2], int (*count_s_d)[2])
{
	while (l[i])
	{
		if (l[i] == '\\' && l[i + 1] && (l[i + 1] == '\'' || l[i + 1] == '\"'))
		{
			i += 2;
			continue ;
		}
		if (l[i] == '\"' && !(*in_quote_s_d)[0])
		{
			if (!(*in_quote_s_d)[1])
				(*in_quote_s_d)[1] = 1;
			else
				(*in_quote_s_d)[1] = 0;
			(*count_s_d)[1]++;
		}
		else if (l[i] == '\'' && !(*in_quote_s_d)[1])
		{
			if (!(*in_quote_s_d)[0])
				(*in_quote_s_d)[0] = 1;
			else
				(*in_quote_s_d)[0] = 0;
			(*count_s_d)[0]++;
		}
		i++;
	}
}

int	check_quotes_closed(char *input)
{
	int	i;
	int	count_s_d[2];
	int	in_quote_s_d[2];

	i = 0;
	count_s_d[0] = 0;
	count_s_d[0] = 0;
	in_quote_s_d[0] = 0;
	in_quote_s_d[0] = 0;
	check_quotes(input, i, &in_quote_s_d, &count_s_d);
	if ((count_s_d[1] % 2 == 0) && (count_s_d[0] % 2 == 0))
		return (SUCCESS);
	return (ERROR);
}
