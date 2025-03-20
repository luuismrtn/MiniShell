/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/20 12:40:25 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char	exit_num = 0;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		return ;
}

void	handle_signal_child(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (sig == SIGQUIT)
		return ;
}

void handle_signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
	}
	else if (sig == SIGQUIT)
		return ;
}

void	signals(char c)
{
	struct sigaction	sa;

	if (c == 'f')
		sa.sa_handler = handle_signal;
	else if (c == 'c')
		sa.sa_handler = handle_signal_child;
	else if (c == 'h')
		sa.sa_handler = handle_signal_heredoc;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction1 \n");
		exit(1);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction2 \n");
		exit(1);
	}
}

void	ign_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction1 \n");
		exit(1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction2 \n");
		exit(1);
	}
}

void	print_2(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("str[%d] = %s\n", i, str[i]);
		i++;
	}
}

/**
 * @brief Cuenta el número de argumentos en un array.
 *
 * Recorre el array de cadenas hasta encontrar un NULL,
 * contando el número total de elementos.
 *
 * @param args Array de cadenas terminado en NULL.
 * @return int El número de elementos en el array.
 */
int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

int	match_string(char *str1, char *str2)
{
	return (ft_strncmp(str1, str2, ft_strlen(str2)) == 0
		&& ft_strlen(str1) == ft_strlen(str2));
}

/**
 * @brief Procesa una parte de la cadena durante la expansión de variables.
 *
 * Esta función auxiliar es llamada por handle_var para procesar cada carácter
 * de la cadena de entrada. Tiene dos modos de operación principales:
 *
 * 1. Para caracteres '$': Extrae el nombre de la variable que sigue, busca
 *    su valor en el entorno, y lo añade al resultado.
 * 2. Para otros caracteres: Los añade directamente al resultado.
 *
 * Manejo de variables:
 * - Si una variable no existe en el entorno, añade una cadena vacía.
 * - Actualiza el índice para saltar el nombre de la variable procesada.
 * - Libera la memoria utilizada para extraer el nombre de la variable.
 *
 * @param tokens Puntero a la estructura con variables de entorno.
 * @param str Cadena que se está procesando.
 * @param i Puntero al índice actual en la cadena (se modifica dentro de la función).
 * @param result Puntero a la cadena resultado (se modifica dentro de la función).
 */
static void	join_result(t_token *tokens, char *str, int *i, char **result)
{
    int		j;
    char	*var_name;
    char	*var_value;

    if (str[(*i)] == '$')
    {
        j = (*i) + 1;
        while (str[j] && str[j] != ':' && str[j] != '$' && str[j] != '=')
            j++;
        var_name = ft_substr(str, (*i) + 1, j - (*i) - 1);
        if (!find_env_var(tokens->env_mshell, var_name))
        {
            (*result) = ft_strjoin((*result), ft_strdup(""));
            (*i) = j - 1;
            return (free(var_name));
        }
        var_value = find_env_var(tokens->env_mshell, var_name)->content;
        free(var_name);
        if (var_value)
            (*result) = ft_strjoin((*result), var_value);
        (*i) = j - 1;
    }
    else
        (*result) = ft_strjoin((*result), ft_substr(str, (*i), 1));
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
 * @return char* Nueva cadena con las variables expandidas o NULL en caso de error.
 *               Esta cadena debe ser liberada por el llamador.
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

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*get_history_path(void)
{
	char	*path;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	path = ft_strjoin(cwd, "/.minishell_history");
	return (path);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_token	*tokens;
	char	*HISTORY_FILE;

	HISTORY_FILE = get_history_path();
	(void)argc;
	(void)argv;
	tokens = malloc(sizeof(t_token));
	ft_memset(tokens, 0, sizeof(t_token));
	tokens->env_mshell = env_buildin(env);
	if (ft_read_history(HISTORY_FILE) == ERROR)
		return (ERROR);
	signals('f');
	tokens->content = ft_strdup("0");
	while (1)
	{
		line = readline(ft_strjoin(find_env_var(tokens->env_mshell, "PWD")->content, " ~ "));
		if (!line)
			break ;
		if (line[0] == '\0' || ft_strtrim(line, " \t\n\r\f\v")[0] == '\0')
		{
			free(line);
			continue ;
		}
		else
		{
			add_history(line);
			write_line_history(HISTORY_FILE, line);
			main2(line, tokens);
			free(line);
		}
		delete_tokens(&tokens);
	}
	return (SUCCESS);
}
