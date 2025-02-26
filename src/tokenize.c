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

t_token	*create_node(t_token_value type, char *content)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	// if (1er nodo) o en todos???
	// 	añadir env;
	// else
	//  env a 0;
	
	new_token->type = type;
	new_token->content = content;
	// if (content[0] == '-')
	// 	new_token->args = content;
	// else
	new_token->args = NULL;
	new_token->env_mshell = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token_value type, char *content)
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

static void	handle_redirections(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(tokens, T_HERE_DOC, ft_strdup("<<"));
			(*i)++;
		}
		else
			add_token(tokens, T_REDIR_LEFT, ft_strdup("<"));
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, T_APPEND, ft_strdup(">>"));
			(*i)++;
		}
		else
			add_token(tokens, T_REDIR_RIGHT, ft_strdup(">"));
		(*i)++;
	}
}

static void	handle_word(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	// if (input[*i] == '-') -> mantener el -
	// 	(*i)++;
	start = *i;
	while (ft_isalpha(input[*i]) || ft_isdigit(input[*i]) || input[*i] == '-')
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, T_WORD, content);
}

static void	handle_spaces(t_token **tokens, char *input, int *i)
{
	int		start;
	char	*content;

	start = *i;
	while (ft_isspace(input[*i]))
		(*i)++;
	content = ft_substr(input, start, *i - start);
	add_token(tokens, T_SPACE, content);
}

int ft_len_var_name(char *str, int i)
{
    while (str[i] && str[i] != ' ' && str[i] != '\"')
        i++;
    return (i);
}


static void	handle_env(t_token **tokens, char *input, int *i)
{
	int		len_var_name;
	char	*var_name;
	t_env	*current_env_list;

	(*i)++;
	len_var_name = ft_len_var_name(input, *i);
	var_name = ft_substr(input, *i, len_var_name);
	current_env_list = (*tokens)->env_mshell;
	while (current_env_list != NULL)
	{
		if (ft_strncmp(current_env_list->name, var_name,
				len_var_name) == SUCCESS)
		{
			free(var_name);
			add_token(tokens, T_ENV, current_env_list->content);
			break ;
		}
		current_env_list = current_env_list->next;
	}
	(*i) += len_var_name - 1;
}

static void	handle_pipe(t_token **tokens, char input, t_token_value type)
{
	char	quotes[2];

	quotes[0] = input;
	quotes[1] = '\0';
	add_token(tokens, type, ft_strdup(quotes));
}

int	num_quotes(char *str, int i, t_token_value type)
{
	int	count;

	count = 0;
	while (str[i])
	{
		if (type == T_D_QUOTE && str[i] == '\"')
			count++;
		else if (type == T_S_QUOTE && str[i] == '\'')
			count++;
		i++;
	}
	return (count);
}

t_result	len_in_quotes(t_token_value type, char *input, int i,
		t_token **tokens)
{
	t_result	data;
	int			count;
	int			count_quotes;
	int			total_quotes;
	int			len_var_name;
	char		*var_name;
	t_env		*current_env_list;

	count = 0;
	count_quotes = 0;
	ft_memset(&data, 0, sizeof(t_result));
	if (type == T_D_QUOTE)
	{
		total_quotes = num_quotes(input, i, type);
		while (count_quotes < total_quotes) // input[i] != "\""
		{
			if (input[i] == '\"')
			{
				count_quotes++;
				if (count_quotes == total_quotes)
				{
					data.len = count;
					return (data);
				}
				i++;
			}
			if (input[i] == '$')
			{
				i++;
				len_var_name = ft_len_var_name(input, i);
				var_name = ft_substr(input, i, len_var_name);
				current_env_list = (*tokens)->env_mshell;
				while (current_env_list != NULL)
				{
					if (ft_strncmp(current_env_list->name, var_name,
							len_var_name) == SUCCESS)
					{
						data.content = ft_strdup(current_env_list->content);
						count += ft_strlen(data.content);
						break ;
					}
					current_env_list = current_env_list->next;
				}
				free(var_name);
			}
			count++;
			i++;
		}
	}
	else if (type == T_S_QUOTE)
	{
		total_quotes = num_quotes(input, i, type);
		while (count_quotes < total_quotes)
		{
			if (input[i] == '\'')
			{
				count_quotes++;
				if (count_quotes == total_quotes)
				{
					data.len = count;
					return (data);
				}
				i++;
			}
			count++;
			i++;
		}
	}
	return (data);
}

static void	handle_quotes(t_token **tokens, char *input, int *i,
		t_token_value type)
{
	char		*in_quotes;
	int			j;
	int			x;
	t_result	data;

	(*i)++;
	if (type == T_D_QUOTE)
	{
		x = 0;
		j = 0;
		data = len_in_quotes(type, input, *i, tokens);
		in_quotes = malloc((data.len + 1) * sizeof(char));
		while (j < data.len) // input[*i] != '\"'
		{
			if (input[*i] == '$') /// acceder a data.content
			{
				(*i) += ft_len_var_name(input, *i) + 1;
				while (data.content[x] != '\0')
					in_quotes[j++] = data.content[x++];
			}
			if (input[*i] == '\"')
				(*i)++;
			in_quotes[j] = input[*i];
			(*i)++;
			j++;
		}
		in_quotes[j] = '\0';
		if (data.len > 0)
			add_token(tokens, T_WORD, in_quotes); /// no olvidar sumar a (*i)
	}
	else if (type == T_S_QUOTE)
	{
		j = 0;
		data = len_in_quotes(type, input, *i, tokens);
		in_quotes = malloc((data.len + 1) * sizeof(char));
		while (j < data.len) // input[*i] != '\''
		{
			if (input[*i] == '\'')
				(*i)++;
			in_quotes[j] = input[*i];
			(*i)++;
			j++;
		}
		in_quotes[j] = '\0';
		if (data.len > 0)
			add_token(tokens, T_WORD, in_quotes);
	}
}

t_token	*tokenize(char *input, char **env)
{
	int		i;
	t_token	*tokens;

	if (!input)
		return (NULL);
	i = 0;
	tokens = malloc(sizeof(t_token));
	if (!tokens)
		return NULL;
	ft_memset(tokens, 0, sizeof(t_token));
	tokens->env_mshell = env_buildin(env);
	while (input[i])
	{
		if (ft_isspace(input[i]))
			handle_spaces(&tokens, input, &i);
		else if (ft_isalpha(input[i]) || ft_isdigit(input[i])
			|| input[i] == '-')
			handle_word(&tokens, input, &i);
		else if (input[i] == '\'')
			handle_quotes(&tokens, input, &i, T_S_QUOTE);
				// handle_quotes(&tokens, input[i++], T_S_QUOTE);
		else if (input[i] == '\"')
			handle_quotes(&tokens, input, &i, T_D_QUOTE);
				// handle_quotes(&tokens, input[i++], T_D_QUOTE);
		else if (input[i] == '<' || input[i] == '>')
			handle_redirections(&tokens, input, &i);
		else if (input[i] == '|')
			handle_pipe(&tokens, input[i], T_PIPE); // handle_quotes(&tokens, input[i++], T_PIPE);
		else if (input[i] == '$')
			handle_env(&tokens, input, &i);
		else
			i++;
	}
	return (tokens);
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
			aux->content = ft_strjoin(aux->content, aux->next->content);
			aux->next = aux->next->next;
		}
		else
		{
			aux = aux->next;
			aux->content = "\0";
			aux->type = T_WORD;
		}
	}
}

int	check_quotes(char *input)
{
	int	i;
	int	count_s;
	int	count_d;

	i = 0;
	count_s = 0;
	count_d = 0;
	while (input[i])
	{
		if (input[i] == '\"')
			count_d++;
		else if (input[i] == '\'')
			count_s++;
		i++;
	}
	if ((count_d % 2 == 0) && (count_s % 2 == 0))
		return (SUCCESS);
	return (ERROR);
}

int	main(int argc, char **argv, char **env)
{
	t_token	*tokens;
	t_token	*aux;

	(void)argc;
	(void)argv;                     
		//"echo \"Hola mundo\"  kek  j l ++ > archivo.txt"
	char *input = argv[1]; //"ca't' -e $USER"
	tokens = NULL;
	if (check_quotes(input) == ERROR)
		exit(1);
	tokens = tokenize(input, env);
	aux = tokens;
	aux = aux->next;
	while (aux != NULL)
	{
		printf("Token type: %d, content: %s\n", aux->type, aux->content);
		aux = aux->next;
	}
	t_token *aux1 = tokens->next;
	clean_tokens(&tokens);
	printf("\n\n");
	while (aux1 != NULL)
	{
		printf("C_Token type: %d, Content: %s\n", aux1->type, aux1->content);
		aux1 = aux1->next;
	}
	free_tokens(tokens);
}
