/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:25:22 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/27 16:07:58 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Maneja la redirección de salida (>)
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
int	handle_redir_right(t_token *tokens, int (*fds)[2])
{
	int	new_fd;

	new_fd = open(tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_fd == -1)
		return (perror("minishell: open"), ERROR);
	if ((*fds)[1] != STDOUT_FILENO)
		close((*fds)[1]);
	return (new_fd);
}

/**
 * @brief Maneja la redirección de salida con modo append (>>)
 *
 * Abre un archivo para escritura, pero añade contenido al final en lugar
 * de truncarlo. Si el archivo no existe, lo crea con permisos 0644.
 *
 * Si ya había una redirección previa, cierra el descriptor anterior.
 *
 * @param tokens El token actual con la redirección
 * @param fds Array de descriptores de archivo [stdin, stdout]
 * @return int El nuevo descriptor para la salida estándar, o ERROR si falla
 */
int	handle_redir_append(t_token *tokens, int (*fds)[2])
{
	int	new_fd;

	new_fd = open(tokens->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (new_fd == -1)
		return (perror("minishell: open"), ERROR);
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
		return (perror("minishell: open"), ERROR);
	if ((*fds)[0] != STDIN_FILENO)
		close((*fds)[0]);
	return (new_fd);
}

/**
 * @brief Maneja la redirección de tipo heredoc (<<)
 *
 * Crea un proceso hijo que lee la entrada estándar hasta encontrar
 * el delimitador especificado. Todo el contenido leído se pasa al
 * proceso padre a través de una tubería.
 *
 * Configura señales específicas para el manejo del heredoc y restaura
 * la configuración original después.
 *
 * @param tokens El token actual con la redirección
 * @return int Descriptor de archivo para leer el contenido del heredoc
 */
int	handle_redir_heredoc(t_token *tokens, t_token *head_tokens)
{
	int		status;
	pid_t	pid;
	int		connect[2];

	ign_signal();
	if (pipe(connect) == -1)
		return (perror("pipe"), ERROR);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), ERROR);
	else if (pid == 0)
	{
		signals('h');
		close(connect[0]);
		handle_heredoc(tokens->next->content, connect[1], head_tokens);
		close(connect[1]);
		exit(0);
	}
	close(connect[1]);
	waitpid(pid, &status, 0);
	signals('f');
	return (connect[0]);
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
void	setup_redirections(t_token *tokens, int (*fds)[2], int count)
{
	t_token *head_tokens;

	head_tokens = tokens;
	tokens = tokens->next;
	while (count > 0)
	{
		if (tokens->type == T_PIPE)
			count--;
		tokens = tokens->next;
	}
	while (tokens)
	{
		if (tokens->type == T_REDIR_RIGHT && tokens->next)
			(*fds)[1] = handle_redir_right(tokens, fds);
		else if (tokens->type == T_APPEND && tokens->next)
			(*fds)[1] = handle_redir_append(tokens, fds);
		else if (tokens->type == T_REDIR_LEFT && tokens->next)
			(*fds)[0] = handle_redir_left(tokens, fds);
		else if (tokens->type == T_HERE_DOC && tokens->next)
			(*fds)[0] = handle_redir_heredoc(tokens, head_tokens);
		else if (tokens && tokens->type == T_PIPE)
			break ;
		tokens = tokens->next;
	}
}
