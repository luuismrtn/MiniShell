/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:49:00 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/04 18:52:39 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Ejecuta un comando, ya sea un builtin o un programa externo
 *
 * Esta función se encarga de ejecutar el comando especificado en el proceso
 * hijo. Determina si el comando es un builtin o un programa externo y actúa
 * en consecuencia. Si es una nueva instancia de minishell, incrementa la
 * variable SHLVL.
 *
 * @param tokens Doble puntero al token que contiene la información de entorno
 * @param fds Puntero al array de descriptores [entrada, salida] para
 * redirección
 * @param args Array de argumentos del comando
 * @param original_stdout Descriptor original de salida estándar
 */
void	executor(t_token **tokens, int (*fds)[2], char **args,
		int original_stdout)
{
	if (match_string(args[0], ""))
	{
		ft_putstr_fd("minishell: '' command not found\n", 2);
		g_exit_num = 127;
		exit(g_exit_num);
	}
	if (match_string(args[0], (*tokens)->name_prog) && num_pipes(*tokens) == 0)
		modify_shlvl(tokens, "SHLVL");
	if (is_builtin(args) == 1)
	{
		change_fds_redir(fds, NULL, &original_stdout, 1);
		handle_builtin(args, (*tokens));
	}
	else
	{
		change_fds_redir(fds, NULL, NULL, 0);
		exe((*tokens), args, original_stdout);
	}
	free_array(args);
	exit(g_exit_num);
}

/**
 * @brief Procesa el último comando en una tubería
 *
 * Esta función maneja el comando final en una tubería.
 * Configura las redirecciones para que:
 * - La entrada estándar reciba datos del comando anterior
 * - La salida estándar mantenga su comportamiento normal o se redirija según
 *   las especificaciones del usuario
 *
 * A diferencia de los comandos intermedios, espera a que este comando termine
 * y actualiza el código de salida.
 *
 * @param count Puntero al contador de comandos (se usa para construir
 * argumentos)
 * @param tokens Doble puntero al token que contiene la información del comando
 * @param fd_in Descriptor de archivo para la entrada desde el comando anterior
 */
int	final_command(int *count, t_token **tokens, int fd_in)
{
	int		fds[2];
	char	**args;
	pid_t	pid;
	int		status;

	set_fds(&fds);
	if (setup_redirections((*tokens), &fds, *count) < 0)
		return (0);
	args = build_command_string((*tokens), *count);
	if (!args || !args[0])
		return (0);
	signals('c');
	if (match_string(args[0], (*tokens)->name_prog))
		ign_signal();
	pid = fork();
	if (pid == -1)
		return (errors_pipex(&fd_in, NULL, args, 'e'));
	else if (pid == 0)
	{
		child_pipe_fdin_redir(&fd_in, NULL);
		executor(tokens, &fds, args, dup(STDOUT_FILENO));
	}
	waitpid(pid, &status, 0);
	g_exit_num = WEXITSTATUS(status);
	return (close(fd_in), free_array(args), 0);
}

/**
 * @brief Procesa el primer comando en una tubería
 *
 * Esta función maneja el primer comando en una tubería.
 * Crea una tubería para conectar este comando con el siguiente y
 * configura las redirecciones para que la salida estándar envíe
 * datos al siguiente comando.
 *
 * @param tokens Doble puntero al token que contiene la información del comando
 * @param count Índice del comando actual (0 para el primero)
 * @return int Descriptor de lectura de la tubería para el siguiente comando
 */
int	first_command(t_token **tokens, int count)
{
	int		fds[2];
	int		connect[2];
	char	**args;
	pid_t	pid;
	int		original_stdout;

	set_fds(&fds);
	if (setup_redirections(*tokens, &fds, count) < 0)
		return (fds[0]);
	args = build_command_string(*tokens, count);
	if (!args || !args[0])
		return (-1);
	if (pipe(connect) == -1)
		return (errors_pipex(NULL, NULL, args, 'p'), -1);
	if (match_string(args[0], (*tokens)->name_prog))
		return (close(connect[1]), connect[0]);
	pid = fork();
	if (pid == -1)
		return (errors_pipex(&connect[0], &connect[1], args, 'f'), -1);
	else if (pid == 0)
	{
		original_stdout = child_pipe_fdin_redir(NULL, &connect);
		executor(tokens, &fds, args, original_stdout);
	}
	return (close(connect[1]), clean_father_material(&fds, args), connect[0]);
}

/**
 * @brief Procesa un comando intermedio en una tubería
 *
 * Esta función maneja los comandos que no son ni el primero ni el último
 * en una tubería. Crea una nueva tubería para conectar este comando con
 * el siguiente, y configura las redirecciones para que:
 * - La entrada estándar reciba datos del comando anterior
 * - La salida estándar envíe datos al siguiente comando
 *
 * @param count Puntero al contador de comandos (se usa para construir
 * argumentos)
 * @param tokens Doble puntero al token que contiene la información del comando
 * @param fd_in Descriptor de archivo para la entrada desde el comando anterior
 * @return int Descriptor de lectura de la nueva tubería para el siguiente
 * comando
 */
int	middle_command(int *count, t_token **tokens, int fd_in)
{
	int		fds[2];
	int		connect[2];
	char	**args;
	pid_t	pid;
	int		original_stdout;

	set_fds(&fds);
	if (setup_redirections(*tokens, &fds, *count) < 0)
		return (close(fd_in), fds[0]);
	args = build_command_string((*tokens), *count);
	if (!args || !args[0])
		return (close(fd_in), -1);
	if (pipe(connect) == -1)
		return (errors_pipex(NULL, NULL, args, 'p'), -1);
	pid = fork();
	if (pid == -1)
		return (errors_pipex(&connect[0], &connect[1], args, 'f'), -1);
	else if (pid == 0)
	{
		original_stdout = child_pipe_fdin_redir(&fd_in, &connect);
		executor(tokens, &fds, args, original_stdout);
	}
	clean_father_material(&fds, args);
	return (close(fd_in), close(connect[1]), connect[0]);
}

/**
 * @brief Ejecuta una serie de comandos conectados mediante tuberías
 *
 * Esta función coordina la ejecución de múltiples comandos conectados por
 * pipes. Divide el proceso en tres fases:
 * 1. Ejecutar el primer comando y crear la primera tubería
 * 2. Ejecutar los comandos intermedios, conectando las tuberías entre ellos
 * 3. Ejecutar el último comando, recibiendo datos del penúltimo
 *
 * Al final, espera a que todos los procesos hijos terminen y restaura
 * los manejadores de señal originales.
 *
 * @param tokens Token que contiene la información de todos los comandos
 * @param num_commands Número total de comandos a ejecutar
 */
void	pipex(t_token *tokens, int num_commands)
{
	int	fd_in;
	int	count;
	int	status;

	fd_in = first_command(&tokens, 0);
	if (num_commands > 1)
	{
		count = 1;
		while (count < num_commands - 1)
		{
			fd_in = middle_command(&count, &tokens, fd_in);
			count++;
		}
		if (count < num_commands)
			final_command(&count, &tokens, fd_in);
		count = 0;
		while (count < num_commands)
		{
			waitpid(-1, &status, 0);
			count++;
		}
		signals('f');
	}
}
