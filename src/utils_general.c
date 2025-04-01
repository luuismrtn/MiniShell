/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:58:28 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/01 01:47:14 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	num_pipes(t_token *tokens)
{
	t_token	*aux;
	int		num;

	aux = tokens->next;
	if (aux == NULL)
		return (0);
	num = 0;
	while (aux != NULL)
	{
		if (aux->type == T_PIPE)
			num++;
		aux = aux->next;
	}
	return (num);
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

int	len_array(char **array)
{
	int	count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}

void	print_2(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("str[%d] = %s\n", i, str[i]);
		i++;
	}
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	match_string(char *str1, char *str2)
{
	return (ft_strncmp(str1, str2, ft_strlen(str2)) == 0
		&& ft_strlen(str1) == ft_strlen(str2));
}

int	check_var_exist(char *var, t_token *tokens)
{
	t_env	*current;

	current = tokens->env_mshell;
	while (current)
	{
		if (match_string(current->name, var))
			return (1);
		current = current->next;
	}
	return (0);
}
