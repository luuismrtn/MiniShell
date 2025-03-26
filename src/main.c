/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/26 20:26:11 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char	exit_num = 0;

int	match_string(char *str1, char *str2)
{
	return (ft_strncmp(str1, str2, ft_strlen(str2)) == 0
		&& ft_strlen(str1) == ft_strlen(str2));
}

static void	join_result(t_token *tokens, char *str, int *i, char **result)
{
	int		j;
	char	*var_name;
	char	*var_value;
	char	*temp;
	t_env	*env_var;
	char	*substr;

	if (str[(*i)] == '$')
	{
		j = (*i) + 1;
		while (str[j] && str[j] != ':' && str[j] != '$' && str[j] != '='
			&& str[j] != ' ' && str[j] != '\t')
			j++;
		var_name = ft_substr(str, (*i) + 1, j - (*i) - 1);
		if (!var_name)
			return ;
		env_var = find_env_var(tokens->env_mshell, var_name);
		if (!env_var)
		{
			free(var_name);
			return ;
		}
		var_value = env_var->content;
		if (var_value)
		{
			temp = *result;
			*result = ft_strjoin(*result, var_value);
			free(temp);
		}
		free(var_name);
		(*i) = j - 1;
	}
	else
	{
		temp = *result;
		substr = ft_substr(str, (*i), 1);
		if (substr)
		{
			*result = ft_strjoin(*result, substr);
			free(substr);
			free(temp);
		}
	}
}

/**
 * @brief Expande las variables de entorno en una cadena.
 *
 * Busca patrones como $VARIABLE en la cadena y los reemplaza por sus valores.
 * Por ejemplo, si la variable "a" contiene "hola", entonces "$a:/home" se
 * convertirá en "hola:/home". Soporta múltiples variables en una cadena.
 *
 * @param str La cadena original que puede contener referencias a variables.
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @return char* Nueva cadena con las variables expandidas o NULLen caso de
 * error. Esta cadena debe ser liberada por el llamador.
 */
char	*handle_env_var(char *str, t_token *tokens)
{
	int		i;
	char	*result;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		join_result(tokens, str, &i, &result);
		i++;
	}
	return (result);
}

char	*get_history_path(void)
{
	char	*path;
	char	*cwd;
	char	*temp;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	temp = ft_strjoin(cwd, "/.minishell_history");
	free(cwd);
	path = temp;
	return (path);
}

/**
 * @brief Libera los recursos asociados a una estructura t_token
 *
 * Libera la lista enlazada de variables de entorno, el contenido del token
 * y finalmente el token mismo.
 *
 * @param tokens Puntero al token que se va a liberar.
 */
void	free_tokens(t_token *tokens)
{
	t_env	*aux;
	t_env	*next;

	if (!tokens)
		return ;
	aux = tokens->env_mshell;
	while (aux)
	{
		next = aux->next;
		free(aux->name);
		free(aux->content);
		free(aux);
		aux = next;
	}
	if (tokens->content)
		free(tokens->content);
	free(tokens);
}

/**
 * @brief Punto de entrada secundario para el procesamiento de comandos
 *
 * Procesa una cadena de entrada como un comando de shell:
 * 1. Verifica que las comillas estén correctamente cerradas
 * 2. Tokeniza la entrada
 * 3. Limpia y optimiza los tokens
 * 4. Analiza la sintaxis con el autómata
 * 5. Ejecuta el comando o pipeline según corresponda
 *
 * @param string Cadena de entrada que contiene el comando a procesar
 * @param tokens Token inicial donde se almacenarán los tokens procesados
 * @return int 0 en caso de éxito, ERROR en caso de error
 */
int	main2(char *string, t_token *tokens)
{
	char	*input;
	int		result;
	int		num_commands;

	input = string;
	if (check_quotes_closed(input) == ERROR)
	{
		printf("Error: quotes not closed\n");
		return (ERROR);
	}
	tokens = tokenize(input, tokens);
	if (!tokens)
		return (ERROR);
	clean_tokens(&tokens);
	result = automata(tokens);
	if (result == 0)
	{
		num_commands = num_pipes(input) + 1;
		if (num_commands == 1)
			one_comnd(&tokens);
		else
			pipex(tokens, num_commands);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_token	*tokens;
	char	*HISTORY_FILE;
	char	*prompt;
	char	*pwd_content;
	char	*trimmed;

	(void)argc;
	(void)argv;
	HISTORY_FILE = get_history_path();
	tokens = malloc(sizeof(t_token));
	ft_memset(tokens, 0, sizeof(t_token));
	tokens->env_mshell = env_buildin(env);
	if (ft_read_history(HISTORY_FILE) == ERROR)
	{
		free(HISTORY_FILE);
		free_tokens(tokens);
		return (ERROR);
	}
	signals('f');
	tokens->content = ft_strdup("0");
	while (1)
	{
		pwd_content = find_env_var(tokens->env_mshell, "PWD")->content;
		prompt = ft_strjoin(pwd_content, " ~ ");
		line = readline(prompt);
		free(prompt);
		if (!line)
			break ;
		trimmed = ft_strtrim(line, " \t\n\r\f\v");
		if (!trimmed || trimmed[0] == '\0')
		{
			free(line);
			free(trimmed);
			continue ;
		}
		free(trimmed);
		add_history(line);
		write_line_history(HISTORY_FILE, line);
		main2(line, tokens);
		free(line);
		delete_tokens(&tokens);
	}
	free(HISTORY_FILE);
	free_tokens(tokens);
	return (SUCCESS);
}
