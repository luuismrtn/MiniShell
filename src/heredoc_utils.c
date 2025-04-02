/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:55:17 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/02 20:01:09 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Concatena un fragmento de línea al contenido acumulado
 *
 * Esta función toma un fragmento específico de la línea de entrada
 * desde la posición 'start' hasta 'i', y lo añade al contenido acumulado.
 * Si el contenido acumulado está vacío (NULL), crea una nueva cadena.
 *
 * @param content Puntero al puntero de la cadena de contenido acumulado
 * @param line La línea completa de donde se extraerá el fragmento
 * @param i La posición final (exclusiva) del fragmento a extraer
 * @param start La posición inicial (inclusiva) del fragmento a extraer
 */
static void	join_content(char **content, char *line, int i, int start)
{
	char	*temp;

	if (*content == NULL)
		(*content) = ft_substr(line, 0, i);
	else
	{
		temp = ft_strjoin(*content, ft_substr(line, start, i - start));
		free((*content));
		(*content) = temp;
	}
}

/**
 * @brief Maneja la expansión de la variable especial $?
 *
 * Esta función expande la variable especial $? reemplazándola por
 * el código de salida de la última ejecución (g_exit_num).
 * Actualiza la posición actual y el inicio del próximo segmento.
 *
 * @param content Puntero al puntero de la cadena de contenido acumulado
 * @param i_start Array de dos enteros: posición actual e inicio del próximo
 * segmento
 */
static void	handle_var_exitnum(char **content, int (*i_start)[2])
{
	char	*temp;

	(*i_start)[0]++;
	temp = ft_strjoin(*content, ft_itoa(g_exit_num));
	free((*content));
	(*content) = temp;
	(*i_start)[1] = (*i_start)[0];
}

/**
 * @brief Obtiene el contenido de una variable de entorno
 *
 * Esta función busca una variable de entorno en la lista de entorno del shell
 * y devuelve su contenido. También actualiza el índice de inicio después del
 * nombre de la variable para continuar el procesamiento.
 *
 * @param tokens Token que contiene el entorno de la shell
 * @param line La línea que contiene la referencia a la variable
 * @param i La posición donde comienza el nombre de la variable
 * @param start_after_d Puntero donde se almacenará la posición después
 * del nombre
 * @return char* El contenido de la variable o NULL si no se encuentra
 */
static char	*get_env_content(t_token *tokens, char *line, int i,
		int *start_after_d)
{
	int		len_var_name;
	char	*var_name;
	char	*var_content;
	t_env	*current;

	var_content = NULL;
	len_var_name = ft_len_var_name(line, i);
	var_name = ft_substr(line, i, len_var_name);
	current = tokens->env_mshell;
	while (current != NULL)
	{
		if (ft_strncmp(current->name, var_name, len_var_name + 1) == SUCCESS)
		{
			free(var_name);
			var_content = ft_strdup(current->content);
			break ;
		}
		current = current->next;
	}
	(*start_after_d) = i + len_var_name;
	return (var_content);
}

/**
 * @brief Expande una variable de entorno en el contenido acumulado
 *
 * Esta función busca el valor de una variable de entorno y lo añade
 * al contenido acumulado. Actualiza las posiciones de inicio para
 * continuar procesando la línea correctamente.
 *
 * @param content Puntero al puntero de la cadena de contenido acumulado
 * @param tokens Token que contiene el entorno de la shell
 * @param line La línea que contiene la referencia a la variable
 * @param i_start Array de dos enteros: posición actual e inicio del próximo
 * segmento
 */
static void	content_variable(char **content, t_token *tokens, char *line,
		int (*i_start)[2])
{
	char	*var_content;
	char	*temp;

	var_content = get_env_content(tokens, line, (*i_start)[0], &(*i_start)[1]);
	temp = ft_strjoin(*content, var_content);
	free((*content));
	free(var_content);
	(*content) = temp;
}

/**
 * @brief Expande todas las variables en una línea de un heredoc
 *
 * Esta función procesa una línea completa de un heredoc,
 * identificando y expandiendo todas las variables ($VAR y $?).
 * La línea original se reemplaza por la versión con las variables expandidas.
 *
 * El proceso:
 * 1. Recorre cada carácter de la línea
 * 2. Cuando encuentra un '$', determina si es $? o una variable de entorno
 * 3. Expande la variable según corresponda
 * 4. Construye una nueva línea con todas las expansiones
 *
 * @param line Puntero al puntero de la línea a procesar y reemplazar
 * @param tokens Token que contiene el entorno de la shell
 */
void	expand_in_heredoc(char **line, t_token *tokens)
{
	int		i_start[2];
	char	*content;
	char	*temp;

	i_start[0] = 0;
	i_start[1] = 0;
	content = NULL;
	while (line[0][i_start[0]])
	{
		if (line[0][i_start[0]] == '$')
		{
			join_content(&content, line[0], i_start[0], i_start[1]);
			i_start[0]++;
			if (line[0][i_start[0]] == '?')
				handle_var_exitnum(&content, &i_start);
			else
				content_variable(&content, tokens, line[0], &i_start);
		}
		i_start[0]++;
	}
	temp = ft_strjoin(content, ft_substr(line[0], i_start[1], i_start[0]
				- i_start[1]));
	free(content);
	(*line) = temp;
}
