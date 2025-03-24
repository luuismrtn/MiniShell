/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:30:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/24 23:43:04 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Crea un nuevo array de comandos combinando partes divididas.
 *
 * Toma un array de partes de comando divididas y el array de comando original,
 * y los combina en un nuevo array, manteniendo el orden correcto.
 *
 * @param split_parts Array de partes del primer argumento.
 * @param orig_cmd Array original de argumentos.
 * @return char** Nuevo array combinado (debe liberarse después).
 */
static char	**make_new_cmd(char **split_parts, char **orig_cmd)
{
	char	**new_cmd;
	int		parts_count;
	int		orig_count;
	int		i;
	int		j;

	parts_count = len_array(split_parts);
	orig_count = len_array(orig_cmd);
	new_cmd = malloc(sizeof(char *) * (parts_count + orig_count));
	if (!new_cmd)
		return (NULL);
	i = 0;
	while (split_parts[i])
	{
		new_cmd[i] = split_parts[i];
		i++;
	}
	j = 1;
	while (orig_cmd[j])
		new_cmd[i++] = orig_cmd[j++];
	new_cmd[i] = NULL;
	return (new_cmd);
}

/**
 * @brief Procesa un comando para manejar argumentos en el primer token.
 *
 * Si el primer argumento contiene espacios (como "ls -l"),
 * lo divide y reorganiza el array de argumentos correctamente.
 *
 * @param cmd Puntero al array de strings del comando.
 */
static void	parse_cmd(char ***cmd)
{
	char	*first_part;
	char	**split_parts;
	char	**new_cmd;
	int		i;

	if (!cmd || !*cmd || !(*cmd)[0])
		return ;
	first_part = ft_strdup((*cmd)[0]);
	if (!first_part)
		return ;
	i = 0;
	while (first_part[i] && !ft_isspace(first_part[i]))
		i++;
	if (!first_part[i])
		return (free(first_part));
	split_parts = ft_split(first_part, ' ');
	free(first_part);
	if (!split_parts)
		return ;
	new_cmd = make_new_cmd(split_parts, *cmd);
	free(split_parts);
	free((*cmd)[0]);
	free(*cmd);
	*cmd = new_cmd;
}

/**
 * @brief Maneja errores de ejecución y termina el proceso.
 *
 * Muestra mensajes de error apropiados según el tipo de problema
 * encontrado y establece el código de salida adecuado.
 *
 * @param cmd Comando que falló.
 */
static void	handle_error_exit(char *cmd)
{
	struct stat	statbuf;

	if (ft_strncmp(cmd, "./", 2) == 0)
	{
		if (stat(cmd, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
			printf("%s: Is a directory\n", cmd);
		else if (access(cmd, F_OK) != 0)
			printf("%s: No such file or directory\n", cmd);
		exit_num = 126;
	}
	else
	{
		printf("%s: command not found\n", cmd);
		exit_num = 127;
	}
	exit(exit_num);
}

/**
 * @brief Ejecuta un comando externo.
 *
 * Procesa, busca y ejecuta un comando externo. Primero intenta ejecutarlo
 * como ruta absoluta/relativa, y luego busca en PATH. Si todo falla,
 * muestra el error apropiado y termina el proceso.
 *
 * @param tokens Estructura de tokens que contiene el entorno.
 * @param cmd Array de argumentos del comando.
 * @param stdout_fd Descriptor de archivo para la salida estándar.
 */
void	exe(t_token *tokens, char **cmd, int stdout_fd)
{
	char	**env;

	parse_cmd(&cmd);
	print_2(cmd);
	env = join_env(tokens->env_mshell);
	if (ft_strchr(cmd[0], '/') != NULL && access(cmd[0], X_OK) == 0)
	{
		close(stdout_fd);
		execve(cmd[0], cmd, env);
	}
	else
		try_exec_with_path(cmd[0], cmd, env);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	handle_error_exit(cmd[0]);
}

/**
 * @brief Verifica si un comando es un builtin.
 *
 * Determina si el comando dado es uno de los comandos internos
 * del shell (echo, cd, pwd, env, export, unset, exit).
 *
 * @param cmd Array de argumentos del comando.
 * @return int 1 si es builtin, 0 si no lo es.
 */
int	is_builtin(char **cmd)
{
	if (!cmd || !cmd[0])
		return (0);
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd[0], "exit", 5) == 0)
		return (1);
	return (0);
}
