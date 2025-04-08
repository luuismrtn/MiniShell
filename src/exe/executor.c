/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:29 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/08 20:59:30 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Ejecuta un comando en un proceso padre
 *
 * Esta función se encarga de ejecutar un comando en el proceso padre.
 * Configura las redirecciones de entrada/salida según sea necesario y
 * ejecuta el comando con execve().
 *
 * @param tokens Lista de tokens que contiene el comando a ejecutar
 * @param fds Array de descriptores de archivo [stdin, stdout]
 * @param args Array de argumentos del comando
 * @param original_stdout Descriptor original de stdout
 */
void	executor_father(t_token *tokens, int (*fds)[2], char ***args,
		int original_stdout)
{
	int	original_stdin;

	original_stdin = -1;
	change_fds_redir(fds, &original_stdin, &original_stdout, 1);
	handle_builtin((*args), tokens);
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	if (original_stdout != -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
	free_array((*args));
}

/**
 * @brief Ejecuta solo un comando
 *
 * Esta función se encarga de ejecutar un comando que no contiene pipes.
 * Configura las redirecciones de entrada/salida según sea necesario y
 * ejecuta el comando con execve().
 *
 * @param tokens Lista de tokens que contiene el comando a ejecutar
 */
void	one_comnd(t_token **tokens)
{
	int		fds[2];
	char	**args;
	pid_t	pid;
	int		status;

	set_fds(&fds);
	if (setup_redirections(*tokens, &fds, 0) < 0)
		return ;
	args = build_command_string(*tokens, 0);
	if (!args || !args[0])
		return ;
	if (is_builtin(args) == 1)
		return (executor_father((*tokens), &fds, &args, -1));
	signals('c');
	if (match_string(args[0], (*tokens)->name_prog))
		ign_signal();
	pid = fork();
	if (pid == -1)
		return (perror("fork"));
	else if (pid == 0)
		executor(tokens, &fds, args, dup(STDOUT_FILENO));
	clean_father_material(&fds, args);
	waitpid(pid, &status, 0);
	g_exit_num = WEXITSTATUS(status);
	signals('f');
}
