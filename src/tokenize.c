/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:53:45 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/21 21:56:17 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token *create_node(t_token_value type, char *content)
{
    t_token *new_token;
    
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->type = type;
    new_token->content = content;
    new_token->next = NULL;
    return (new_token);
}

static void add_token(t_token **head, t_token_value type, char *content)
{
    t_token *aux;
    t_token *new_token = create_node(type, content);

    if (!new_token || !head)
		return (free(new_token));
	if (*head == NULL)
		*head = new_token;
	else
	{
        aux = *head;
		while (aux->next != NULL)
            aux = aux->next;
		aux->next = new_token;
	}
}

t_token *tokenize(char *input)
{
    int i;
    int start;
    char buffer[200];
    t_token *tokens = NULL;

    i = 0;
    //tokens = ft_memset(tokens, 0, sizeof(t_token *)); porq da segfault esto :?
    while (input[i] != '\0')
    {
        if (ft_isspace(input[i]) == 1)
        {
            start = i;
            while (ft_isspace(input[i]) == 1)
                i++;
            ft_strlcpy(buffer, &input[start], i - start + 1);
            add_token(&tokens, IS_SPACE, buffer);
            continue;
        }
        if (ft_isalpha(input[i]) || ft_isdigit(input[i]) || (input[i] == '-' && input[i + 1] == ft_isalpha(input[i])))
        {
            if (input[i] == '-')
                i++;
            start = i;
            while (ft_isalpha(input[i]) || ft_isdigit(input[i]))
                i++;
            ft_strlcpy(buffer, &input[start], i - start + 1);
            add_token(&tokens, WORD, buffer);
            continue;
        }
        if (input[i] == '\'')
        {
            add_token(&tokens, S_QUOTE, "\'");
            i++;
            continue;
        }
        if (input[i] == '\"')
        {
            add_token(&tokens, D_QUOTE, "\"");
            i++;
            continue;
        }
        if (input[i] == '<')
        {
            if (input[i + 1] == '<')
            {
                add_token(&tokens, HERE_DOC, "<<");
                i += 2;
            }
            else
            {
                add_token(&tokens, REDIR_LEFT, "<");
                i++;
            }
            continue;
        }
        if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                add_token(&tokens, APPEND, ">>");
                i += 2;
            }
            else
            {
                add_token(&tokens, REDIR_RIGHT, ">");
                i++;
            }
            continue;
        }
        if (input[i] == '|')
        {
            add_token(&tokens, PIPE, "|");
            i++;
            continue;
        }
        if (input[i] == '$')
        {
            start = ++i;
            while (ft_isalpha(input[i]) || input[i] == '(' || input[i] == ')')
                i++;
            ft_strlcpy(buffer, &input[start], i - start + 1);
            add_token(&tokens, ENV, buffer);
            continue;
        }
        i++;
    }
    return (tokens);
}

int main() //contenido con cosas raras + muchos leaaaksss
{
    char input[] = "echo \"Hola mundo\"  caca  j l ++ > archivo.txt";
    t_token *tokens = NULL;

    tokens = tokenize(input);
    t_token *aux = tokens;
    while (aux != NULL)
    {
        printf("Token type: %d, Content: %s\n", aux->type, aux->content);
        aux = aux->next;
    }
}