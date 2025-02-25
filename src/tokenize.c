/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:53:45 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/25 17:26:40 by aldferna         ###   ########.fr       */
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
	if (content[0] == '-')
		new_token->args = content;
	else
		new_token->args = NULL;
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
	while (str[i] != ' ' || str[i] != '\"')
		i++;
	return i;
}

static void	handle_env(t_token **tokens, char *input, int *i) //guardar el valor expandido
{
	// int		start;
	// char	*content;

	// start = ++(*i);
	// while (ft_isalpha(input[*i]))
	// 	(*i)++;
	// content = ft_substr(input, start, *i - start);

	int len_var_name;
	char *var_name;
	t_env *current_env_list;
	int x;
	int j;
	char *content;

	(*i)++;
	j = 0;
	while (input[*i] && (input[*i] != ' ' || input[*i] != '\"'))
	{
		len_var_name = ft_len_var_name(input, *i);
		var_name = ft_substr(input, *i, len_var_name);
		current_env_list = (*tokens)->env_mshell;
		while (current_env_list != NULL)
		{
			x = 0;
			if (ft_strncmp(current_env_list->name, var_name, len_var_name) == SUCCESS)
			{
				while(current_env_list->content[x] != '\0')
					content[j++] = current_env_list->content[x++];
				break;
			}
			current_env_list = current_env_list->next;
		}
		free(var_name);
	}
	add_token(tokens, T_ENV, content);
}

// static void	handle_quotes(t_token **tokens, char input, t_token_value type)
// {
// 	char	quotes[2];

// 	quotes[0] = input;
// 	quotes[1] = '\0';
// 	add_token(tokens, type, ft_strdup(quotes));
// }

static void	handle_pipe(t_token **tokens, char input, t_token_value type)
{
	char	quotes[2];

	quotes[0] = input;
	quotes[1] = '\0';
	add_token(tokens, type, ft_strdup(quotes));
}

char **len_in_quotes(t_token_value type, char *input, int i, t_token **tokens)
{
	char **len_and_content;
	int count;
	int count_quotes;
	int num_of_quotes;

	count = 0;
	count_quotes = 0;
	//num_of_quotes = funcionnnn - 1;
	if (type == T_D_QUOTE)
	{
		//while (input[i] != "\"") //while count quotes < total quotes (lo que devuelva la funcion -1 (x la 1a))
		{
			if (input[i] == '\"')
			{
				i++;
				count_quotes++;
				if (count_quotes == num_of_quotes)
				{
					len_and_content[0] = ft_itoa(count);
					return (len_and_content);
				}
			}
			if (input[i] == '$')
			{
				i++;
				//hacer aqui la busqueda en vez de en ambass funciones y devolver esto tb
			}
			count++;
			i++;
		}
		return count;
	}
	else if (type == T_S_QUOTE)
	{
		
	}
	//y si devuelvo una matriz, en la 1a len y a 2a el contenido de la variable expandidda?
}

static void handle_quotes(t_token **tokens, char *input, int *i, t_token_value type)
{
	char *in_quotes;
	char *var_name;
	int len_var_name;
	int j;
	int x;
	int len; //funncionnn
	t_env *current_env_list;

	(*i)++;
	if (type == T_D_QUOTE)
	{
		j = 0;
		len = len_in_quotes(type, input, *i, tokens);
		in_quotes = malloc((len + 1) * sizeof(char));
		while (j < len) //input[*i] != '\"'  
		{
			if (input[*i] == '$')
			{
				(*i)++;
				len_var_name = ft_len_var_name(input, *i);
				var_name = ft_substr(input, *i, len_var_name);
				current_env_list = (*tokens)->env_mshell;
				while (current_env_list != NULL)
				{
					x = 0;
					if (ft_strncmp(current_env_list->name, var_name, len_var_name) == SUCCESS)
					{
						while(current_env_list->content[x] != '\0')
							in_quotes[j++] = current_env_list->content[x++];
						break;
					}
					current_env_list = current_env_list->next;
				}
				free(var_name);
			}
			if (input[*i] == '\"')
				(*i)++;
			in_quotes[j] = input[*i];
			(*i)++;
			j++;
		}
		in_quotes[j] = '\0';
		add_token(tokens, type, in_quotes);  ///aqui al final habria q sumarle len a (*i)
	}
	else if (type == T_S_QUOTE)
	{
		j = 0;
		len = len_in_quotes(type, input, *i, tokens);
		in_quotes = malloc((len + 1) * sizeof(char));
		while(j < len) //input[*i] != '\''
		{
			if (input[*i] == '\'')
				(*i)++;
			in_quotes[j] = input[*i];
			(*i)++;
			j++;
		}
		in_quotes[j] = '\0';
		add_token(tokens, type, in_quotes);
	}
}

t_token	*tokenize(char *input, char **env)
{
	int		i;
	t_token	*tokens;

	if (!input)
		return (NULL);
	i = 0;
	tokens = ft_memset(&tokens, 0, sizeof(t_token));
	tokens->env_mshell = env_buildin(env);
	while (input[i])
	{
		if (ft_isspace(input[i]))
			handle_spaces(&tokens, input, &i);
		else if (ft_isalpha(input[i]) || ft_isdigit(input[i])
			|| (input[i] == '-' && ft_isalpha(input[i + 1])))
			handle_word(&tokens, input, &i);
		else if (input[i] == '\'')
			handle_quotes(&tokens, input, &i, T_S_QUOTE); //handle_quotes(&tokens, input[i++], T_S_QUOTE);
		else if (input[i] == '\"')
			handle_quotes(&tokens, input, &i, T_D_QUOTE);  //handle_quotes(&tokens, input[i++], T_D_QUOTE);
		else if (input[i] == '<' || input[i] == '>')
			handle_redirections(&tokens, input, &i);
		else if (input[i] == '|')
			handle_pipe(&tokens, input[i++], T_PIPE);  //handle_quotes(&tokens, input[i++], T_PIPE);
		else if (input[i] == '$')
			handle_env(&tokens, input, &i);
		else
			i++;
	}
	return (tokens);
}

void clean_tokens(t_token *tokens)
{
	t_token *aux;

	aux = tokens;
	while (aux != NULL)
	{
		while (aux->type != T_SPACE) //juntarrrr
		{
			
		}
		//borrar nodo token espacio y unir el anterior al siguiente
		aux = aux->next;
	}
}

//esto 1er paso despues de history en main
int check_quotes(char *input)
{
	int i;
	int count_s;
	int count_d;

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
		return SUCCESS;
	return ERROR;
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv; //"echo \"Hola mundo\"  kek  j l ++ > archivo.txt"
	char input[] = "ca't' -e $USER"; //readline
	t_token *tokens = NULL; 

	if (check_quotes(input) == ERROR)
		exit(1);
	tokens = tokenize(input, env);
	t_token *aux = tokens;
	while (aux != NULL)
	{
		printf("Token type: %d, content: %s\n", aux->type, aux->content);
		aux = aux->next;
	}
	
	// clean_tokens(tokens);
	// t_token *aux1 = tokens;
	// while (aux1 != NULL)
	// {
	// 	printf("C_Token type: %d, Content: %s, Expand: %s\n", aux1->type, aux1->content, aux1->expanded);
	// 	aux1 = aux1->next;
	// }
	free_tokens(tokens);
}
