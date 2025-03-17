/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:53:45 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/26 16:38:222 by lumartin         ###   ########.fr       */
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

t_token	*create_node(t_token_value type, char *content, int quotes)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->content = content;
	if (type == T_D_QUOTE)
		new_token->quotes = quotes;
	else
		new_token->quotes = quotes;
	new_token->env_mshell = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token_value type, char *content, int quotes)
{
	t_token	*aux;
	t_token	*new_token;

	new_token = create_node(type, content, quotes);
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

void	delete_tokens(t_token **tokens)
{
	t_env	*env;
	char	*content;
	t_token	*aux;
	t_token	*next;

	env = NULL;
	if (!tokens || !*tokens)
		return ;
	env = (*tokens)->env_mshell;
	content = (*tokens)->content;
	aux = *tokens;
	while (aux != NULL)
	{
		next = aux->next;
		if (aux->content)
			aux->content = NULL;
		free(aux);
		aux = next;
	}
	*tokens = malloc(sizeof(t_token));
	if (!*tokens)
		return ;
	ft_memset(*tokens, 0, sizeof(t_token));
	(*tokens)->env_mshell = env;
	(*tokens)->content = content;
}

static void	handle_redirections(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(tokens, T_HERE_DOC, ft_strdup("<<"), 0);
			(*i)++;
		}
		else
			add_token(tokens, T_REDIR_LEFT, ft_strdup("<"), 0);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, T_APPEND, ft_strdup(">>"), 0);
			(*i)++;
		}
		else
			add_token(tokens, T_REDIR_RIGHT, ft_strdup(">"), 0);
		(*i)++;
	}
}

static void	handle_spaces(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = *i;
	while (ft_isspace(input[*i]))
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, T_SPACE, content, 0);
}

static void	handle_pipe(t_token **tokens, char input, t_token_value type,
		int *i)
{
	char	quotes[2];

	quotes[0] = input;
	quotes[1] = '\0';
	(*i)++;
	add_token(tokens, type, ft_strdup(quotes), 0);
}

void	clean_tokens(t_token **tokens)
{
	t_token	*aux;

	if (!tokens || !*tokens)
		return ;
	aux = *tokens;
	aux = aux->next;
	while (aux != NULL && aux->next != NULL)
	{
		if (aux->type != T_SPACE && aux->next->type != T_SPACE)
		{
			if (aux->type == aux->next->type || (aux->type == T_WORD
					&& aux->next->type == T_ENV) || (aux->type == T_ENV
					&& aux->next->type == T_WORD))
			{
				aux->content = ft_strjoin(aux->content, aux->next->content);
				aux->quotes = aux->next->quotes;
				aux->next = aux->next->next;
			}
			else
				aux = aux->next;
		}
		else
		{
			if (aux->next->next != NULL)
			{
				aux = aux->next;
				aux->content = "\0";
				aux->type = aux->next->type;
			}
			else
				aux = aux->next;
		}
	}
}

int	ft_len_var_name(char *str, int i)
{
	int	count;

	count = 0;
	while (str[i] && (str[i] != ' ' && str[i] != '\"'))
	{
		count++;
		i++;
	}
	return (count);
}

t_result	content_in_quotes(t_token_value type, char *input, int i,
		t_token **tokens)
{
	t_result	data;
	int			len_var_name;
	char		*var_name;
	t_env		*current_env_list;
	char *temp;
	char *content;
	int start;
	char *var_content;
	int count;
	int there_is_expansion;

	count = 0;
	there_is_expansion = 0;
	content = NULL;
	ft_memset(&data, 0, sizeof(t_result));
	if (type == T_D_QUOTE)
	{
		printf("aqui entra: %c\n", input[i]);
		start = i;
		while (input[i] && !(input[i] == '\"' && input[i - 1] != '\\'))
		{
			if (input[i] == '\\' && input[i + 1] == '\"')//PROBAR ESTOOO
			{
				i++; //=+ 2?
				count++;
			}
			else if (input[i] == '$' && input[i + 1] != '\"' && input[i + 1] != ' ')
			{
				there_is_expansion = 1;
				i++;
				count++;
				len_var_name = ft_len_var_name(input, i);
				count += len_var_name;
				var_name = ft_substr(input, i, len_var_name);
				current_env_list = (*tokens)->env_mshell;
				if (input[i] == '?')
				{
					if (content == NULL)
					{
						content = ft_substr(input, start, i - start - 1);
						temp = ft_strjoin(content, ft_itoa(exit_num));
					}
					else
					{
						content = ft_strjoin(temp, ft_substr(input, start, i - start - 1));
						temp = ft_strjoin(content, ft_itoa(exit_num));
					}
					free(content);
					free(var_name);
					start = i + 1;
					continue ;
				}
				while (current_env_list != NULL)
				{
					if (ft_strncmp(current_env_list->name, var_name,
							len_var_name) == SUCCESS)
					{
						var_content = ft_strdup(current_env_list->content);
						if (content == NULL)
						{
							content = ft_substr(input, start, i - start - 1);
							printf("content NULL; %s\n", content);
							temp = ft_strjoin(content, var_content);
						}
						else
						{
							content = ft_strjoin(temp, ft_substr(input, start, i - start - 1));
							printf("content NO-NULL; %s\n", content);
							temp = ft_strjoin(content, var_content);
						}
						free(content);
						free(var_content);
						free(var_name);
						break ;
					}
					current_env_list = current_env_list->next;
				}
				i += len_var_name - 1;
				printf("i apunta despues bucle: %c\n", input[i]);
				start = i + 1;
			}
			i++;
			count++;
		}
		if (there_is_expansion == 1)
			data.content = ft_strjoin(temp, ft_substr(input, start, i - start));
		else
			data.content = ft_substr(input, start, i - start);
		printf("CONTENIDO FINAL: %s\n", data.content);
		data.len = count;
		printf("LEN FINAL: %d\n", data.len);
	}
	else if (type == T_S_QUOTE)
	{
		while (input[i] && !(input[i] == '\'' && input[i - 1] != '\\'))
		{
			count++;
			i++;
		}
		data.len = count;
	}
	return (data);
}

static void	handle_quotes(t_token **tokens, char *input, int *i,
		t_token_value type)
{
	char		*in_quotes;
	int			j;
	//int			x;
	t_result	data;
	//int			len_var;

	(*i)++;
	if (type == T_D_QUOTE)
	{
		data = content_in_quotes(type, input, *i, tokens);
		add_token(tokens, T_WORD, data.content, 1);
		(*i) += data.len;
	}
	else if (type == T_S_QUOTE)
	{
		j = 0;
		data = content_in_quotes(type, input, *i, tokens);
		in_quotes = malloc((data.len + 1) * sizeof(char));
		if (!in_quotes)
			return ;
		while (j < data.len && input[*i])
		{
			in_quotes[j++] = input[*i];
			(*i)++;
		}
		in_quotes[j] = '\0';
		if (input[*i] == '\'')
			(*i)++;
		if (j > 0)
			add_token(tokens, T_WORD, in_quotes, 1);
		else
			free(in_quotes);
	}
}

static void	handle_env(t_token **tokens, char *input, int *i)
{
	int		len_var_name;
	char	*var_name;
	t_env	*current_env_list;

	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		var_name = ft_itoa(exit_num);
		add_token(tokens, T_ENV, var_name, 0);
		return ;
	}
	len_var_name = ft_len_var_name(input, *i);
	var_name = ft_substr(input, *i, len_var_name);
	current_env_list = (*tokens)->env_mshell;
	while (current_env_list != NULL)
	{
		if (ft_strncmp(current_env_list->name, var_name,
				len_var_name) == SUCCESS)
		{
			free(var_name);
			add_token(tokens, T_ENV, current_env_list->content, 0);
			break ;
		}
		current_env_list = current_env_list->next;
	}
	(*i) += len_var_name;
}

static void	handle_word(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = *i;
	if (input[*i] == '\\')
	{
		(*i)++;
		start++;
	}
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\"'
		&& input[*i] != '\'')
		(*i)++;
	content = ft_substr(input, start, *i - start);
	if (content[0] == '-')
		add_token(tokens, T_FLAG, content, 0);
	else
		add_token(tokens, T_WORD, content, 0);
}

t_token	*tokenize(char *input, t_token *tokens)
{
	int	i;

	if (!input)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			handle_spaces(&tokens, input, &i);
		else if (input[i] == '\'')
			handle_quotes(&tokens, input, &i, T_S_QUOTE);
		else if (input[i] == '\"')
			handle_quotes(&tokens, input, &i, T_D_QUOTE);
		else if (input[i] == '<' || input[i] == '>')
			handle_redirections(&tokens, input, &i);
		else if (input[i] == '|')
			handle_pipe(&tokens, input[i], T_PIPE, &i);
		else if (input[i] == '$' && input[i + 1] != '\0')
			handle_env(&tokens, input, &i);
		else
			handle_word(&tokens, input, &i);
	}
	return (tokens);
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

// int	main2(char *string, t_token *tokens)
// {
// 	char	*input;

// 	input = string;
// 	if (check_quotes_closed(input) == ERROR)
// 	{
// 		printf("Error: quotes not closed\n");
// 		return (ERROR);
// 	}
// 	tokens = tokenize(input, tokens);
// 	clean_tokens(&tokens);
// 	if (automata(tokens) == 0)
// 		pipex(input, tokens);	
// 	return (0);
// }

int     main2(char *string, t_token *tokens)
{
        t_token *aux;
        t_token *aux1;
        char    *input;

        input = string;
        printf("Input: %s\n", input);
        if (check_quotes_closed(input) == ERROR)
        {
                printf("Error: quotes not closed\n");
                return (ERROR);
        }
        tokens = tokenize(input, tokens);
        aux = tokens->next;
        while (aux != NULL)
        {
                printf("Token type: %d, content: %s, quote: %d\n", aux->type, aux->content, aux->quotes);
                aux = aux->next;
        }
        aux1 = tokens->next;
        clean_tokens(&tokens);
        printf("\n\n");
        while (aux1 != NULL)
        {
                printf("C_Token type: %d, Content: %s, quote: %d\n", aux1->type, aux1->content, aux1->quotes);
                aux1 = aux1->next;
        }
        if (automata(tokens) == 0)
                pipex(input, tokens);
        return (0);
}
