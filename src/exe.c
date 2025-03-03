/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:30:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/03 20:19:39 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exe(char **env, char **comnd)
{
	char	**paths;
	int		i;

	if ((ft_strchr(comnd[0], '/') != NULL) && (access(comnd[0], X_OK) == 0))
		execve(comnd[0], comnd, env);
	else
	{
		paths = search_path(env, comnd[0]);
		i = 0;
		while (paths[i] != NULL)
		{
			if (access(paths[i], R_OK | X_OK) == 0)
				execve(paths[i], comnd, env);
			i++;
		}
		i = 0;
		while (paths[i])
			free(paths[i++]);
		free(paths);
	}
	write(1, "error command\n", 14);
	exit(127);
}

char	**search_path(char **env, char *comnd)
{
	char	**paths;
	char	*aux;
	char	*new_path;
	int		i;

	i = 0;
	while ((env[i] != NULL) && (ft_strncmp(env[i], "PATH=", 5) != 0))
		i++;
	if (env[i] == NULL)
	{
		write(2, "Path not found\n", 16);
		exit(10);
	}
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		aux = ft_strjoin(paths[i], "/");
		new_path = ft_strjoin(aux, comnd);
		free(paths[i]);
		paths[i] = new_path;
		free(aux);
		i++;
	}
	return (paths);
}
