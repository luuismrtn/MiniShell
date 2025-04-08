/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:25:22 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/08 20:59:21 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Maneja la redirección de salida 'r' (>) y el append 'a' (>>)
 *
 * Abre un archivo para escritura, truncándolo si ya existe o creándolo
 * si no existe. Configura los permisos a 0644 (lectura/escritura para
 * el propietario, solo lectura para grupo y otros).
 *
 * Si ya había una redirección previa, cierra el descriptor anterior.
 *
 * @param tokens El token actual con la redirección
 * @param fds Array de descriptores de archivo [stdin, stdout]
 * @return int El nuevo descriptor para la salida estándar, o ERROR si falla
 */
int	handle_redir_right_append(t_token *tokens, int (*fds)[2], char c)
{
	int	new_fd;

	if (c == 'r')
		new_fd = open(tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (c == 'a')
		new_fd = open(tokens->next->content, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	if (new_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tokens->next->content, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		g_exit_num = 1;
		return (new_fd);
	}
	if ((*fds)[1] != STDOUT_FILENO)
		close((*fds)[1]);
	return (new_fd);
}

/**
 * @brief Maneja la redirección de entrada (<)
 *
 * Abre un archivo para lectura. Si el archivo no existe o no puede abrirse,
 * devuelve un error.
 *
 * Si ya había una redirección previa, cierra el descriptor anterior.
 *
 * @param tokens El token actual con la redirección
 * @param fds Array de descriptores de archivo [stdin, stdout]
 * @return int El nuevo descriptor para la entrada estándar, o ERROR si falla
 */
int	handle_redir_left(t_token *tokens, int (*fds)[2])
{
	int	new_fd;

	new_fd = open(tokens->next->content, O_RDONLY);
	if (new_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tokens->next->content, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		return (new_fd);
	}
	if ((*fds)[0] != STDIN_FILENO)
		close((*fds)[0]);
	return (new_fd);
}

/**
 * @brief Configura las redirecciones de entrada/salida
 *
 * Recorre la lista de tokens y busca los operadores de redirección.
 * Configura los descriptores de archivo apropiados para la entrada
 * y salida estándar del comando.
 *
 * Detiene la búsqueda al encontrar un pipe o al final de la lista
 * de tokens.
 *
 * @param tokens Lista de tokens que contiene el comando y sus redirecciones
 * @param fds Puntero a un array [stdin, stdout] donde se guardan los
 * descriptores
 * @param head_tokens Puntero a la cabeza de la lista de tokens
 */
void	make_redirections(t_token *tokens, int (*fds)[2])
{
	while (tokens)
	{
		if (tokens->type == T_REDIR_RIGHT && tokens->next)
			(*fds)[1] = handle_redir_right_append(tokens, fds, 'r');
		else if (tokens->type == T_APPEND && tokens->next)
			(*fds)[1] = handle_redir_right_append(tokens, fds, 'a');
		else if (tokens->type == T_REDIR_LEFT && tokens->next)
		{
			(*fds)[0] = handle_redir_left(tokens, fds);
			if ((*fds)[0] < 0)
				break ;
		}
		else if (tokens->type == T_PIPE)
			break ;
		tokens = tokens->next;
	}
}

/**
 * @brief Configura todas las redirecciones para un comando
 *
 * Recorre la lista de tokens buscando operadores de redirección
 * y los procesa según su tipo. Configura los descriptores de archivo
 * apropiados para la entrada y salida estándar del comando.
 *
 * Se detiene al encontrar un pipe o al final de la lista de tokens.
 *
 * @param tokens Lista de tokens que contiene el comando y sus redirecciones
 * @param fds Puntero a un array [stdin,
		stdout] donde se guardan los descriptores
 * @param count Contador de pipes para saltar al comando correcto
 */
int	setup_redirections(t_token *tokens, int (*fds)[2], int count)
{
	t_token	*head_tokens;

	head_tokens = tokens;
	tokens = tokens->next;
	while (count > 0)
	{
		if (tokens->type == T_PIPE)
			count--;
		tokens = tokens->next;
	}
	make_redirections(tokens, fds);
	make_heredoc_redir(tokens, fds, head_tokens);
	if ((*fds)[1] < 0 || (*fds)[0] < 0)
		return (-1);
	return (0);
}
