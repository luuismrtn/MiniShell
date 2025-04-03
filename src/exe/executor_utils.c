/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:00:46 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/03 21:45:05 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Inicializa los descriptores de archivo para stdin y stdout
 * 
 * Esta función establece los descriptores de archivo para stdin y stdout
 * en un array de enteros. Se utiliza para inicializar los descriptores
 * antes de realizar operaciones de redirección o creación de pipes.
 * 
 * @param fds Puntero a un array de enteros donde se almacenarán
 */
void	set_fds(int (*fds)[2])
{
	(*fds)[0] = STDIN_FILENO;
	(*fds)[1] = STDOUT_FILENO;
}

/**
 * @brief Maneja errores en el proceso de ejecución de comandos con pipes
 *
 * Esta función gestiona diferentes tipos de errores que pueden ocurrir
 * durante la ejecución de comandos en pipelines. Libera los recursos
 * asignados (array de argumentos) y cierra los descriptores de archivo
 * según sea necesario.
 *
 * @param pipe_in Puntero al descriptor de entrada de la tubería
 * @param pipe_out Puntero al descriptor de salida de la tubería
 * @param args Array de argumentos del comando (se liberará)
 * @param c Carácter que indica el tipo de error:
 *          'p' - 'pipe' Error en pipe()
 *          'f' - 'fork' Error en fork() con descriptor de salida
 *          'e' - 'end' Error en fork() sin descriptor de salida
 *          'i' - 'in' Error si fd_in recibido < 0
 */
int	errors_pipex(int *pipe_in, int *pipe_out, char **args, char c)
{
	int	fd_null;

	free_array(args);
	if (c == 'p')
		perror("pipe");
	else if (c == 'f')
	{
		perror("fork");
		return (close(*pipe_in), close(*pipe_out));
	}
	else if (c == 'e')
		return (perror("fork"), close(*pipe_in));
	else if (c == 'i')
	{
		fd_null = open("/dev/null", O_RDONLY);
		if (fd_null == -1)
		{
			perror("open /dev/null");
			exit(1);
		}
		dup2(fd_null, STDIN_FILENO);
		close(fd_null);
	}
	return (0);
}

/**
 * @brief Configura las redirecciones de entrada/salida para un proceso hijo en
 * un pipeline
 *
 * Esta función:
 * 1. Guarda el descriptor original de stdout
 * 2. Redirecciona stdin al descriptor proporcionado (fd_in), si existe
 * 3. Redirecciona stdout al extremo de escritura de la tubería (connect[1]),
 * si existe
 *
 * @param fd_in Puntero al descriptor de archivo para la entrada
 * @param args Array de argumentos del comando (para manejo de errores)
 * @param connect Puntero a array de descriptores [lectura,
	escritura] de la tubería
 * @return int Descriptor original de stdout (para restaurarlo después)
 */
int	child_pipe_fdin_redir(int *fd_in, int (*connect)[2])
{
	int	original_stdout;

	original_stdout = STDOUT_FILENO;
	if (fd_in != NULL)
	{
		if ((*fd_in) < 0)
			errors_pipex(NULL, NULL, NULL, 'i');
		dup2((*fd_in), STDIN_FILENO);
		close((*fd_in));
	}
	if (connect && ((*connect)[0] || (*connect)[1]))
	{
		close((*connect)[0]);
		original_stdout = dup(STDOUT_FILENO);
		dup2((*connect)[1], STDOUT_FILENO);
		close((*connect)[1]);
	}
	return (original_stdout);
}

/**
 * @brief Cambia los descriptores estándar por los proporcionados
 *
 * Esta función reemplaza stdin y/o stdout por los descriptores especificados,
 * guardando los originales si executor_father es 1. Se usa para implementar
 * redirecciones de entrada/salida en comandos.
 *
 * @param fds Puntero al array de descriptores [entrada, salida]
 * @param o_stdin Puntero donde guardar el descriptor original de stdin
 * @param o_stdout Puntero donde guardar el descriptor original de stdout
 * @param executor_father Indica si se ejecuta en el proceso padre (1) o
 * hijo (0)
 */
void	change_fds_redir(int (*fds)[2], int *o_stdin, int *o_stdout,
		int executor_father)
{
	if ((*fds)[0] != STDIN_FILENO)
	{
		if (executor_father == 1)
			(*o_stdin) = dup(STDIN_FILENO);
		dup2((*fds)[0], STDIN_FILENO);
		close((*fds)[0]);
	}
	if ((*fds)[1] != STDOUT_FILENO)
	{
		if (executor_father == 1)
			(*o_stdout) = dup(STDOUT_FILENO);
		dup2((*fds)[1], STDOUT_FILENO);
		close((*fds)[1]);
	}
}

/**
 * @brief Limpia los recursos utilizados por el proceso padre
 *
 * Esta función se encarga de:
 * 1. Cerrar los descriptores de archivo no estándar
 * 2. Liberar la memoria asignada para los argumentos del comando
 *
 * Se llama normalmente después de que el proceso padre ha terminado
 * de lanzar y esperar a los procesos hijos.
 *
 * @param fds Puntero al array de descriptores [entrada, salida]
 * @param args Array de argumentos del comando (se liberará)
 */
void	clean_father_material(int (*fds)[2], char **args)
{
	if ((*fds)[0] != STDIN_FILENO)
		close((*fds)[0]);
	if ((*fds)[1] != STDOUT_FILENO)
		close((*fds)[1]);
	free_array(args);
}
