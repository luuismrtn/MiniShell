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

t_token	*create_node(t_token_value type, char *content, int quotes)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->content = ft_strdup(content);
	if (!new_token->content)
		return (free(new_token), NULL);
	new_token->quotes = quotes;
	new_token->env_mshell = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token_value type, char *content, int quotes)
{
	t_token	*aux;
	t_token	*new_token;

	if (!content)
		return ;
	new_token = create_node(type, content, quotes);
	free(content);
	if (!new_token || !head)
		return ;
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
	t_env	*var;
	char	*content;
	t_token	*aux;
	t_token	*next;

	env = NULL;
	if (!tokens || !*tokens)
		return ;
	var = (*tokens)->exp_var;
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
	(*tokens)->exp_var = var;
	(*tokens)->content = content;
}

static void	handle_redirections(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i) += 2;
			add_token(tokens, T_HERE_DOC, ft_strdup("<<"), 0);
			while (input[*i] && ft_isspace(input[*i]))
				(*i)++;
			while (input[*i] && (!ft_isspace(input[*i]) && input[*i] != '\"'
					&& input[*i] != '\''))
			{
				add_token(tokens, T_WORD, ft_substr(input, (*i), 1), 0);
				(*i)++;
			}
		}
		else
		{
			(*i)++;
			add_token(tokens, T_REDIR_LEFT, ft_strdup("<"), 0);
		}
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
	t_token	*temp;
	char	*old_content;
	char	*new_content;

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
				old_content = aux->content;
				new_content = ft_strjoin(old_content, aux->next->content);
				if (new_content)
				{
					free(old_content);
					aux->content = new_content;
					if (aux->quotes == 0 && aux->next->quotes == 1)
						aux->quotes = 1;
					temp = aux->next;
					aux->next = aux->next->next;
					free(temp->content);
					free(temp);
				}
				else
					aux = aux->next;
			}
			else
				aux = aux->next;
		}
		else
		{
			if (aux->next->next != NULL)
			{
				aux = aux->next;
				free(aux->content);
				aux->content = ft_strdup("");
				if (!aux->content)
					aux->content = ft_strdup(" ");
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
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
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
	char		*result;
	char		*temp;
	int			start;
	int			j;
	char		*value;
	int			var_start;
	int			var_len;
	char		*var_name;
	t_env		*env;
	char		*var_value;
	char		dollar[2] = "$\0";
	char		c[2];

	ft_memset(&data, 0, sizeof(t_result));
	result = NULL;
	if (type == T_D_QUOTE)
	{
		start = i;
		j = 0;
		while (input[i] && !(input[i] == '\"' && (i == 0 || input[i
					- 1] != '\\')))
		{
			j++;
			i++;
		}
		i = start;
		result = malloc((j + 1) * sizeof(char));
		if (!result)
			return (data);
		result[0] = '\0';
		while (input[i] && !(input[i] == '\"' && (i == 0 || input[i
					- 1] != '\\')))
		{
			if (input[i] == '$' && input[i + 1] && input[i + 1] != '\"'
				&& input[i + 1] != ' ')
			{
				i++;
				if (input[i] == '?')
				{
					value = ft_itoa(exit_num);
					temp = ft_strjoin(result, value);
					free(result);
					free(value);
					result = temp;
					i++;
				}
				else
				{
					var_start = i;
					var_len = 0;
					while (input[i] && (ft_isalnum(input[i])
							|| input[i] == '_'))
					{
						var_len++;
						i++;
					}
					if (var_len > 0)
					{
						var_name = ft_substr(input, var_start, var_len);
						env = (*tokens)->env_mshell;
						var_value = NULL;
						while (env)
						{
							if (ft_strlen(env->name) == ft_strlen(var_name)
								&& ft_strncmp(env->name, var_name,
									ft_strlen(var_name)) == 0)
							{
								var_value = env->content;
								break ;
							}
							env = env->next;
						}
						if (var_value)
							temp = ft_strjoin(result, var_value);
						else
							temp = ft_strjoin(result, "");
						free(result);
						free(var_name);
						result = temp;
					}
					else
					{
						temp = ft_strjoin(result, dollar);
						free(result);
						result = temp;
					}
				}
			}
			else
			{
				c[0] = input[i];
				c[1] = '\0';
				temp = ft_strjoin(result, c);
				free(result);
				result = temp;
				i++;
			}
		}
		data.content = result;
		data.len = i - start;
	}
	else if (type == T_S_QUOTE)
	{
		start = i;
		j = 0;
		while (input[i] && !(input[i] == '\'' && (i == 0 || input[i
				- 1] != '\\')))
		{
			j++;
			i++;
		}
		data.content = ft_substr(input, start, j);
		data.len = j;
	}
	return (data);
}

static void	handle_quotes(t_token **tokens, char *input, int *i,
		t_token_value type)
{
	t_result	data;

	(*i)++;
	if (!input[*i])
	{
		add_token(tokens, T_WORD, ft_strdup(""), 1);
		return ;
	}
	data = content_in_quotes(type, input, *i, tokens);
	if (data.content)
	{
		add_token(tokens, T_WORD, data.content, 1);
		(*i) += data.len;
		if ((type == T_D_QUOTE && input[*i] == '\"') || (type == T_S_QUOTE
				&& input[*i] == '\''))
			(*i)++;
	}
	else
		add_token(tokens, T_WORD, ft_strdup(""), 1);
}

static void	handle_env(t_token **tokens, char *input, int *i)
{
	int		len_var_name;
	char	*var_name;
	char	*var_content;
	t_env	*current_env_list;
	int		found;

	var_content = NULL;
	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		var_name = ft_itoa(exit_num);
		add_token(tokens, T_ENV, var_name, 0);
		return ;
	}
	len_var_name = ft_len_var_name(input, *i);
	if (len_var_name == 0)
	{
		add_token(tokens, T_WORD, ft_strdup("$"), 0);
		return ;
	}
	var_name = ft_substr(input, *i, len_var_name);
	current_env_list = (*tokens)->env_mshell;
	found = 0;
	while (current_env_list != NULL)
	{
		if (ft_strncmp(current_env_list->name, var_name,
				len_var_name) == SUCCESS
			&& (int)ft_strlen(current_env_list->name) == len_var_name)
		{
			found = 1;
			var_content = ft_strdup(current_env_list->content);
			break ;
		}
		current_env_list = current_env_list->next;
	}
	(*i) += len_var_name;
	if (found)
		add_token(tokens, T_ENV, var_content, 0);
	else
		add_token(tokens, T_ENV, ft_strdup(""), 0);
	free(var_name);
	if (input[*i] == '$' && input[*i + 1] != '\0' && (ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '_' || input[*i + 1] == '?'))
		handle_env(tokens, input, i);
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
		&& input[*i] != '\'' && input[*i] != '|' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '$')
		(*i)++;
	content = ft_substr(input, start, *i - start);
	if (!content)
		return ;
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
		// printf("Input[%d]: %c\n", i, input[i]);
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
// 	int		result;

// 	input = string;
// 	if (check_quotes_closed(input) == ERROR)
// 	{
// 		printf("Error: quotes not closed\n");
// 		return (ERROR);
// 	}
// 	tokens = tokenize(input, tokens);
// 	if (!tokens)
// 		return (ERROR);
// 	clean_tokens(&tokens);
// 	result = automata(tokens);
// 	if (result == 0)
// 		pipex(input, tokens);
// 	return (0);
// }

int	main2(char *string, t_token *tokens)
{
	t_token	*aux;
	t_token	*aux1;
	char	*input;

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
		printf("Token type: %d, content: %s, quotes: %d\n", aux->type,
			aux->content, aux->quotes);
		aux = aux->next;
	}
	aux1 = tokens->next;
	clean_tokens(&tokens);
	printf("\n\n");
	while (aux1 != NULL)
	{
		printf("C_Token type: %d, content: %s, quotes: %d\n", aux1->type,
			aux1->content, aux1->quotes);
		aux1 = aux1->next;
	}
	if (automata(tokens) == 0)
	{
		pipex(input, tokens);
	}
	return (0);
}
