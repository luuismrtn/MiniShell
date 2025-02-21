/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:53:45 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/21 22:16:32 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->content);
		free(tokens);
		tokens = next;
	}
}

t_token	*create_node(t_token_value type, char *content)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->content = content;
	new_token->next = NULL;
	return (new_token);
}

static void	add_token(t_token **head, t_token_value type, char *content)
{
	t_token	*aux;
	t_token	*new_token;

	new_token = create_node(type, content);
	if (!new_token || !head)
	{
		free(content);
		free(new_token);
		return ;
	}
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

static void	handle_quotes(t_token **tokens, char input, t_token_value type)
{
	char	quotes[2];

	quotes[0] = input;
	quotes[1] = '\0';
	add_token(tokens, type, ft_strdup(quotes));
}

static void	handle_redirections(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(tokens, HERE_DOC, ft_strdup("<<"));
			(*i)++;
		}
		else
			add_token(tokens, REDIR_LEFT, ft_strdup("<"));
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, APPEND, ft_strdup(">>"));
			(*i)++;
		}
		else
			add_token(tokens, REDIR_RIGHT, ft_strdup(">"));
		(*i)++;
	}
}

static void	handle_word(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	if (input[*i] == '-')
		(*i)++;
	start = *i;
	while (ft_isalpha(input[*i]) || ft_isdigit(input[*i]))
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, WORD, content);
}

static void	handle_spaces(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = *i;
	while (ft_isspace(input[*i]))
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, IS_SPACE, content);
}

static void	handle_env(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = ++(*i);
	while (ft_isalpha(input[*i]) || input[*i] == '(' || input[*i] == ')')
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, ENV, content);
}

t_token	*tokenize(char *input)
{
	int		i;
	t_token	*tokens;

	if (!input)
		return (NULL);
	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			handle_spaces(&tokens, input, &i);
		else if (ft_isalpha(input[i]) || ft_isdigit(input[i])
			|| (input[i] == '-' && ft_isalpha(input[i + 1])))
			handle_word(&tokens, input, &i);
		else if (input[i] == '\'')
			handle_quotes(&tokens, input[i++], S_QUOTE);
		else if (input[i] == '\"')
			handle_quotes(&tokens, input[i++], D_QUOTE);
		else if (input[i] == '<' || input[i] == '>')
			handle_redirections(&tokens, input, &i);
		else if (input[i] == '|')
			handle_quotes(&tokens, input[i++], PIPE);
		else if (input[i] == '$')
			handle_env(&tokens, input, &i);
		else
			i++;
	}
	return (tokens);
}

int	main(void)
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
	free_tokens(tokens);
}
