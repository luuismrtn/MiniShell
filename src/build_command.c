/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:26:41 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/26 19:41:45 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	count_args_for_comnd(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_WORD || tokens->type == T_FLAG
			|| tokens->type == T_ENV)
			count++;
		if (tokens->type == T_PIPE)
			break ;
		tokens = tokens->next;
	}
	return (count);
}

static void fill_args(char **args, int num_args, t_token *tokens)
{
    int i;

	i = 0;
    while ((i < num_args) && tokens)
    {
        if (tokens->type == T_WORD || tokens->type == T_FLAG 
		|| tokens->type == T_ENV)
        {
            args[i] = ft_strdup(tokens->content);
            if (!args[i])
                return (free(args));
            i++;
        }
        if (tokens && tokens->type == T_PIPE)
            break;
        if (tokens->type == T_REDIR_RIGHT || tokens->type == T_APPEND ||
            tokens->type == T_REDIR_LEFT || tokens->type == T_HERE_DOC)
        {
            tokens = tokens->next->next;
            continue;
        }
        tokens = tokens->next;
    }
    args[i] = NULL;
}

char	**build_command_string(t_token *tokens, int count)
{
	char	**args;
	int		num_args;

	tokens = tokens->next;
	while (count > 0) // && temp_tokens->type (condicion q daba problemas)
	{
		if (tokens->type == T_PIPE)
			count--;
		tokens = tokens->next;
	}
	num_args = count_args_for_comnd(tokens);
	args = malloc((num_args + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	fill_args(args, num_args, tokens);
	return (args);
}
