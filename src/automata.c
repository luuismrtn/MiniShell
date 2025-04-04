/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/04 18:18:55 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Muestra un mensaje de error de sintaxis
 *
 * Esta función muestra un mensaje de error según el tipo de token
 * que causó el problema en la sintaxis. También establece el código
 * de salida a 1 para indicar un error.
 *
 * Los tipos de token posibles son:
 * 0: Comando
 * 1: Flag/Argumento
 * 2: Pipe
 * 3-6: Redirecciones (<, >, <<, >>)
 * 7: Error
 *
 * @param prev_token Tipo de token que causó el error
 */
static void	print_automat_error(int prev_token)
{
	char	*elements[9];

	elements[0] = "command";
	elements[1] = "flag";
	elements[2] = "|";
	elements[3] = "newline";
	elements[4] = "newline";
	elements[5] = "newline";
	elements[6] = "newline";
	elements[7] = "err";
	printf("syntax error: %s\n", elements[prev_token]);
	g_exit_num = 2;
}

/**
 * @brief Crea la matriz de transición del autómata
 *
 * Genera una matriz representada como un array de enteros donde:
 * - Cada fila corresponde a un estado del autómata
 * - Cada columna corresponde a un tipo de token
 * - Los valores indican el estado siguiente tras la transición
 *
 * @return int** Matriz de transición del autómata
 */
static int	**create_automata(void)
{
	int	**automata;

	automata = malloc(sizeof(int *) * 6);
	if (!automata)
		return (NULL);
	automata[0] = (int []){1, 5, 5, 3, 3, 3, 3, 2};
	automata[1] = (int []){2, 1, 4, 3, 3, 3, 3, 1};
	automata[2] = (int []){2, 2, 4, 3, 3, 3, 3, 2};
	automata[3] = (int []){2, 2, 5, 5, 5, 5, 5, 2};
	automata[4] = (int []){1, 5, 5, 3, 3, 3, 3, 2};
	automata[5] = (int []){5, 5, 5, 5, 5, 5, 5, 5};
	return (automata);
}

/**
 * @brief Verifica la sintaxis de un comando mediante un autómata finito
 *
 * Esta función implementa un autómata finito determinista para
 * verificar si la secuencia de tokens forma una expresión sintácticamente
 * válida según las reglas de la shell.
 *
 * Estados del autómata:
 * 0: Estado inicial
 * 1: Después de un comando
 * 2: Después de un archivo
 * 3: Después de una redirección
 * 4: Después de un pipe
 * 5: Error
 *
 * Transiciones según el tipo de token:
 * columnas: [WORD, FLAG, PIPE, REDIR_LEFT, REDIR_RIGHT, HERE_DOC, APPEND,
	ERROR]
 * filas: estados [INICIAL, COMANDO, ARCHIVO, REDIRECCION, PIPE, ERROR]
 *
 * @param tokens Lista de tokens a verificar
 * @return int 0 si la sintaxis es correcta, 1 si hay error
 */
int	automata(t_token *tokens)
{
	int	current_state;
	int	prev_token;
	int	**automata;

	prev_token = 0;
	automata = create_automata();
	if (!automata || tokens->next == NULL)
		return (free(automata), 1);
	tokens = tokens->next;
	current_state = 0;
	while (current_state != 5 && tokens != NULL)
	{
		prev_token = tokens->type;
		current_state = automata[current_state][prev_token];
		tokens = tokens->next;
	}
	if ((current_state != 1 && current_state != 2))
		print_automat_error(prev_token);
	free(automata);
	return ((current_state != 1 && current_state != 2));
}
