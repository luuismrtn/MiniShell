/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:55:17 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/27 16:45:23 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void join_content(char **content, char *line, int i, int start)
{
    char *temp;

    if (*content == NULL)
        (*content) = ft_substr(line, 0, i);
    else
    {
        temp = ft_strjoin(*content, ft_substr(line, start, i - start));
        free((*content));
        (*content) = temp;
    }
}

void handle_var_exitnum(char **content, int (*i_start)[2])
{
    char *temp;
    
    (*i_start)[0]++;
    temp = ft_strjoin(*content, ft_itoa(exit_num));
    free((*content));
    (*content) = temp;
    (*i_start)[1] = (*i_start)[0];
}

char	*get_env_content(t_token *tokens, char *line, int i, int *start_after_d)
{
	int		len_var_name;
	char	*var_name;
	char	*var_content;
	t_env	*current;

	var_content = NULL;
	len_var_name = ft_len_var_name(line, i);
	var_name = ft_substr(line, i, len_var_name);
	current = tokens->env_mshell;
	while (current != NULL)
	{
		if (ft_strncmp(current->name, var_name, len_var_name + 1) == SUCCESS)
		{
			free(var_name);
			var_content = ft_strdup(current->content);
			break ;
		}
		current = current->next;
	}
	(*start_after_d) = i + len_var_name;
	return (var_content);
}

void content_variable(char **content, t_token *tokens, char *line, int (*i_start)[2])
{
    char *var_content;
    char *temp;
    
    var_content = get_env_content(tokens, line, (*i_start)[0], &(*i_start)[1]);
    temp = ft_strjoin(*content, var_content);
    free((*content));
    free(var_content);
    (*content) = temp;
}

void expand_in_heredoc(char **line, t_token *tokens)
{
    int i_start[2];
    char *content;
    char *temp;

    i_start[0] = 0;
    i_start[1] = 0;
    content = NULL;
    while (line[0][i_start[0]])
    {
        if (line[0][i_start[0]] == '$')
        {
            join_content(&content, line[0], i_start[0], i_start[1]);
            i_start[0]++;
            if (line[0][i_start[0]] == '?')
                handle_var_exitnum(&content, &i_start);
            else
                content_variable(&content, tokens, line[0], &i_start);
        }
        i_start[0]++;
    }
    temp = ft_strjoin(content, ft_substr(line[0], i_start[1], i_start[0] - i_start[1]));
    free(content);
    (*line) = temp;
}
