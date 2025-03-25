/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:58:28 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/25 21:42:52 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	num_pipes(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count++;
		i++;
	}
	return (count);
}

int	has_pipe(t_token *tokens)
{
	t_token	*aux;

	aux = tokens->next;
	while (aux != NULL)
	{
		if (aux->type == T_PIPE)
			return (1);
		aux = aux->next;
	}
	return (0);
}

int	ft_len_var_name(char *str, int i)
{
	int	count;

	count = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		count++;
		i++;
	}
	return (count);
}

int	check_quotes_closed(char *input)
{
	int	i;
	int	count_s;
	int	count_d;
	int	in_d_quote;
	int	in_s_quote;

	i = 0;
	count_s = 0;
	count_d = 0;
	in_d_quote = 0;
	in_s_quote = 0;
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] && (input[i + 1] == '\'' || input[i
				+ 1] == '\"'))
		{
			i += 2;
			continue ;
		}
		if (input[i] == '\"' && !in_s_quote)
		{
			if (!in_d_quote)
				in_d_quote = 1;
			else
				in_d_quote = 0;
			count_d++;
		}
		else if (input[i] == '\'' && !in_d_quote)
		{
			if (!in_s_quote)
				in_s_quote = 1;
			else
				in_s_quote = 0;
			count_s++;
		}
		i++;
	}
	if ((count_d % 2 == 0) && (count_s % 2 == 0))
		return (SUCCESS);
	return (ERROR);
}

int	len_array(char **array)
{
	int	count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}
