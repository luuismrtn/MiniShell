/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:26:41 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/03 21:28:17 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Cuenta el número de argumentos para un comando
 *
 * Recorre la lista de tokens hasta encontrar un pipe o llegar al final,
 * contando los tokens que representan palabras, flags o variables de entorno,
 * que son los que formarán parte de los argumentos del comando.
 *
 * @param tokens Puntero al primer token del comando
 * @return int Número de argumentos encontrados para el comando
 */
static int	count_args_for_comnd(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_WORD || tokens->type == T_FLAG
			|| tokens->type == T_ENV)
			count++;
		if (tokens->type == T_PIPE)
			break ;
		tokens = tokens->next;
	}
	return (count);
}

/**
 * @brief Rellena un array de strings con los argumentos del comando
 *
 * Recorre la lista de tokens y copia el contenido de los tokens relevantes
 * (palabras, flags y variables de entorno) al array de argumentos.
 * Maneja correctamente las redirecciones saltándolas junto con sus archivos.
 * Finaliza el array con un NULL como marca de fin.
 *
 * @param args Array de strings donde guardar los argumentos
 * @param num_args Número máximo de argumentos a copiar
 * @param tokens Puntero al primer token del comando
 */
static void	fill_args(char **args, int num_args, t_token *tokens)
{
	int	i;

	i = 0;
	while ((i < num_args) && tokens)
	{
		if (tokens->type == T_WORD || tokens->type == T_FLAG
			|| tokens->type == T_ENV)
		{
			args[i] = ft_strdup(tokens->content);
			if (!args[i])
				return (free(args));
			i++;
		}
		if (tokens && tokens->type == T_PIPE)
			break ;
		if (tokens->type == T_REDIR_RIGHT || tokens->type == T_APPEND
			|| tokens->type == T_REDIR_LEFT || tokens->type == T_HERE_DOC)
		{
			tokens = tokens->next->next;
			continue ;
		}
		tokens = tokens->next;
	}
	args[i] = NULL;
}

/**
 * @brief Construye un array de strings con los argumentos de un comando
 *
 * Esta función principal prepara un array de cadenas en el formato adecuado
 * para pasarlo a execve() o a un builtin.
 *
 * El primer elemento del array (args[0]) será el nombre del comando,
 * seguido por sus argumentos, y terminando con NULL.
 *
 * @param tokens Puntero al token inicial (cabeza de la lista)
 * @param count Índice del comando a construir (0 para el primero,
 * 1 para el segundo, etc.)
 * @return char** Array de strings con el comando y sus argumentos,
 * NULL si hay error
 */
char	**build_command_string(t_token *tokens, int count)
{
	char	**args;
	int		num_args;

	tokens = tokens->next;
	while (count > 0)
	{
		if (tokens->type == T_PIPE)
			count--;
		tokens = tokens->next;
	}
	num_args = count_args_for_comnd(tokens);
	args = malloc((num_args + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	fill_args(args, num_args, tokens);
	if (!args[0])
	{
		free_array(args);
		return (NULL);
	}
	return (args);
}
