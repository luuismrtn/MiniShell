/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path_exe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:39:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/09 16:24:45 by lumartin         ###   ########.fr       */
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
	char	*path_str;
	int		i;

	paths = search_path(env, cmd);
	path_str = NULL;
	i = 0;
	while (paths[i] != NULL)
	{
		path_str = ft_strdup(paths[i]);
		if (access(path_str, R_OK | X_OK) == 0)
		{
			free_array(paths);
			if (!env)
				return (perror("env"));
			execve(path_str, args, env);
		}
		free(path_str);
		i++;
	}
	free_array(paths);
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
	char	**paths_split;
	char	*path_str;
	int		i;

	path_str = get_path_from_env(env);
	paths_split = ft_split(path_str, ':');
	free(path_str);
	paths = malloc(sizeof(char *) * (len_array(paths_split) + 1));
	i = 0;
	while (paths_split[i] != NULL)
	{
		paths[i] = join_path_and_cmd(paths_split[i], cmd);
		i++;
	}
	paths[i] = NULL;
	free_array(paths_split);
	return (paths);
}
