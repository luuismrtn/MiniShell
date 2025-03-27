/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/27 01:37:10 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Código de salida del último comando ejecutado
 *
 * Esta variable global almacena el código de salida del último comando
 * ejecutado.
 */
unsigned char	exit_num = 0;

/**
 * @brief Obtiene la ruta completa al archivo de historial
 *
 * Genera la ruta al archivo de historial basándose en el directorio de trabajo
 * actual. El archivo se llama ".minishell_history" y se guarda en el directorio
 * desde donde se ejecutó el shell.
 *
 * @return char* Ruta completa al archivo de historial
 */
static char	*get_history_path(void)
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
	free(cwd);
	return (path);
}

/**
 * @brief Procesa y ejecuta una línea de comando
 *
 * Esta función implementa el flujo principal de procesamiento de un comando:
 * 1. Verifica que las comillas estén correctamente cerradas
 * 2. Tokeniza la línea de entrada
 * 3. Limpia y optimiza los tokens
 * 4. Verifica la sintaxis mediante el autómata
 * 5. Ejecuta el comando o pipeline según corresponda
 *
 * @param line Línea de comando a procesar
 * @param tokens Token inicial con información de entorno
 * @return int 0 en caso de éxito, ERROR en caso de error
 */
static int	run(char *line, t_token *tokens)
{
	int	result;
	int	num_commands;

	if (check_quotes_closed(line) == ERROR)
	{
		printf("Error: quotes not closed\n");
		return (ERROR);
	}
	tokens = tokenize(line, tokens);
	if (!tokens)
		return (ERROR);
	clean_tokens(&tokens);
	result = automata(tokens);
	if (result == 0)
	{
		num_commands = num_pipes(tokens) + 1;
		if (num_commands == 1)
			one_comnd(&tokens);
		else
			pipex(tokens, num_commands);
	}
	return (0);
}

/**
 * @brief Procesa una línea de entrada del usuario
 *
 * Realiza el preprocesamiento de la línea ingresada:
 * 1. Elimina espacios en blanco iniciales y finales
 * 2. Verifica si la línea está vacía
 * 3. Añade la línea al historial
 * 4. Ejecuta la línea como comando
 * 5. Limpia los recursos utilizados
 *
 * @param line Línea de entrada a procesar
 * @param tokens Doble puntero al token con información de entorno
 * @param history_file Ruta al archivo de historial
 */
static void	process_line(char *line, t_token **tokens, char *history_file)
{
	char	*trimmed;

	trimmed = ft_strtrim(line, " \t\n\r\f\v");
	if (!trimmed || trimmed[0] == '\0')
	{
		free(line);
		free(trimmed);
		return ;
	}
	free(trimmed);
	add_history(line);
	write_line_history(history_file, line);
	if (run(line, *tokens) == ERROR)
	{
		free(line);
		delete_tokens(tokens);
		exit(1);
	}
	free(line);
	delete_tokens(tokens);
}

/**
 * @brief Muestra el prompt personalizado y lee la entrada del usuario
 *
 * Genera un prompt que muestra el directorio de trabajo actual,
 * lo muestra al usuario y lee una línea de entrada mediante readline.
 *
 * @param tokens Token con información de entorno
 * @return char* Línea leída del usuario (NULL si hay error o EOF)
 */
static char	*show_prompt(t_token *tokens)
{
	char	*pwd_content;
	char	*prompt;
	char	*line;

	pwd_content = find_env_var(tokens->env_mshell, "PWD")->content;
	prompt = ft_strjoin(pwd_content, " ~ ");
	line = readline(prompt);
	free(prompt);
	return (line);
}

/**
 * @brief Punto de entrada principal del programa
 *
 * Inicializa el shell, configura el entorno y el historial,
 * y entra en el bucle principal que lee y procesa comandos
 * hasta que el usuario salga del programa.
 *
 * @param argc Número de argumentos de línea de comandos
 * @param argv Array de argumentos de línea de comandos
 * @param env Variables de entorno del sistema
 * @return int SUCCESS en caso de éxito, ERROR en caso de error
 */
int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_token	*tokens;
	char	*history_file;

	(void)argc;
	(void)argv;
	history_file = get_history_path();
	tokens = initialize_shell(env);
	if (!tokens || ft_read_history(history_file) == ERROR)
	{
		free(history_file);
		free_tokens(tokens);
		return (ERROR);
	}
	while (1)
	{
		line = show_prompt(tokens);
		if (!line)
			break ;
		process_line(line, &tokens, history_file);
	}
	free(history_file);
	free_tokens(tokens);
	return (SUCCESS);
}
