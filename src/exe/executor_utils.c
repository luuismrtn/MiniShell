/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:00:46 by aldferna          #+#    #+#             */
/*   Updated: 2025/03/28 16:44:14 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
 *          'p' - Error en pipe()
 *          'f' - Error en fork() con descriptor de salida
 *          'e' - Error en fork() sin descriptor de salida
 *          'd' - Error en archivo de salida
 */
void	errors_pipex(int *pipe_in, int *pipe_out, char **args, char c)
{
	free_array(args);
	if (c == 'p')
		perror("pipe");
	else if (c == 'f')
	{
		perror("fork");
		close((*pipe_in));
		close((*pipe_out));
	}
	else if (c == 'e')
	{
		perror("fork");
		close((*pipe_in));
	}
	else if (c == 'd')
	{
		perror("error output file");
		exit(1);
	}
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
int	child_pipe_fdin_redir(int *fd_in, char **args, int (*connect)[2])
{
	int	original_stdout;

	original_stdout = STDOUT_FILENO;
	if (fd_in != NULL)
	{
		if ((*fd_in) < 0)
			return(errors_pipex(NULL, NULL, args, 'd'), ERROR);
		printf("que pasa\n");
		printf("stdin: %d\n", STDIN_FILENO);
		dup2((*fd_in), STDIN_FILENO);
		printf("stdin despues: %d\n", STDIN_FILENO);
		printf("1\n");
		//close((*fd_in));
		printf("2\n");
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
		printf("entra aqui\n");
		if (executor_father == 1)
			(*o_stdin) = dup(STDIN_FILENO);
		dup2((*fds)[0], STDIN_FILENO);
		close((*fds)[0]);
	}
	if ((*fds)[1] != STDOUT_FILENO)
	{
		printf("o entra aqui\n");
		if (executor_father == 1)
			(*o_stdout) = dup(STDOUT_FILENO);
		// dup2((*fds)[1], STDOUT_FILENO);
		// close((*fds)[1]);
		printf("HEHE fds[1]: %d\n", (*fds)[1]);
		if ((*fds)[1] >= 0)
		{
			if (dup2((*fds)[1], STDIN_FILENO) == -1)
			{
				perror("Error en dup2 para stdin");
				exit(EXIT_FAILURE);
			}
			close((*fds)[1]);
		}
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
