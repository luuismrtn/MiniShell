/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:53:45 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/24 19:46:53 by aldferna         ###   ########.fr       */
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

	// if (input[*i] == '-')
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

static void	handle_env(t_token **tokens, char *input, int *i) //guardar el valor expandido
{
	int		start;
	char	*content;

	start = ++(*i);
	while (ft_isalpha(input[*i])) // || input[*i] == '(' || input[*i] == ')' --> echo $(USER) = USER cmd not found
		(*i)++;
	content = ft_substr(input, start, *i - start);
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

static void handle_quotes(t_token **tokens, char *input, int *i, t_token_value type)
{
	int start;
	char *in_quotes;
	int j;
	int len; //funncionnnn

	(*i)++;
	start = *i;
	if (type == T_D_QUOTE)
	{
		while (input[*i] != '\"')
		{
			if (input[*i] == '$')
			{
				(*i)++;
				//expandir
			}
			//ir guardando - obviar " dentro
			//sustituir $loqsea despues?? o meter en in_quotes cd aparezca en input?
		}
	}
	else if (type == T_S_QUOTE)
	{
		j = 0;
		in_quotes = malloc(len * sizeof(char));
		while(input[*i] != '\'')
		{
			if (input[*i] != '\'')
				(*i)++;
			in_quotes[j] = input[*i];
			(*i)++;
			j++;
		}
		add_token(tokens, type, in_quotes);
	}
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
	(void)argv;
	char input[] = "ca't' -e $USER"; //"echo \"Hola mundo\"  kek  j l ++ > archivo.txt"
	t_token *tokens = NULL;
	t_env *env_mshell = NULL;

	if (check_quotes(input) == ERROR)
		exit(1);
	env_mshell = env_buildin(env);
	// t_env *aux2 = env_mshell;
	// while (aux2)
	// {
	// 	printf("Name: %s\n  content: %s\n", aux2->name, aux2->content);
	// 	aux2 = aux2->next;
	// }
	
	tokens = tokenize(input);
	t_token *aux = tokens;
	while (aux != NULL)
	{
		printf("Token type: %d, content: %s, expand: %s\n", aux->type, aux->content, aux->expanded);
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
