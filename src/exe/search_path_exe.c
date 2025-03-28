/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path_exe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:39:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/28 14:08:20 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Intenta ejecutar un comando buscando en los directorios de PATH.
 *
 * Construye todas las posibles rutas para el comando y trata
 * de ejecutarlo en cada ubicación hasta encontrar una válida.
 *
 * @param cmd Nombre del comando a ejecutar.
 * @param args Array de argumentos completo.
 * @param env Variables de entorno.
 */
void	try_exec_with_path(char *cmd, char **args, char **env)
{
	char	**paths;
	int		i;

	paths = search_path(env, cmd);
	i = 0;
	while (paths[i] != NULL)
	{
		if (access(paths[i], R_OK | X_OK) == 0)
		{
			if (!env)
				return(perror("env"));
			printf("paths: %s\n", paths[i]);
			print_2(args);
			execve(paths[i], args, env);
		printf("try_exec_wiht\n");
		}
		i++;
	}
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

/**
 * @brief Une una ruta base con un comando.
 *
 * Concatena un directorio de PATH con un comando,
 * insertando una barra entre ellos.
 *
 * @param path Ruta base (directorio).
 * @param cmd Nombre del comando.
 * @return char* Ruta completa (debe ser liberada por el llamador).
 */
static char	*join_path_and_cmd(char *path, char *cmd)
{
	char	*with_slash;
	char	*result;

	with_slash = ft_strjoin(path, "/");
	result = ft_strjoin(with_slash, cmd);
	free(with_slash);
	return (result);
}

/**
 * @brief Busca todas las posibles ubicaciones de un comando.
 *
 * Construye un array de posibles rutas completas para un comando
 * basándose en los directorios definidos en PATH.
 *
 * @param env Array de variables de entorno.
 * @param cmd Nombre del comando a buscar.
 * @return char** Array de posibles rutas (debe liberarse después).
 */
char	**search_path(char **env, char *cmd)
{
	char	**paths;
	char	*path_str;
	int		i;

	path_str = get_path_from_env(env);
	paths = ft_split(path_str, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		paths[i] = join_path_and_cmd(paths[i], cmd);
		i++;
	}
	return (paths);
}
