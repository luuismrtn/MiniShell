/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:09:26 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/20 15:20:21 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

int	ok_args(int argc, char **arg)
{
	int	i;
	int	x;
	int	count;

	count = 0;
	i = 2;
	while (i < argc - 1)
	{
		x = 0;
		while (arg[i][x] != '\0')
		{
			if (arg[i][x] != ' ' && arg[i][x] != '\t' && arg[i][x] != '\n')
			{
				count++;
				break ;
			}
			x++;
		}
		i++;
	}
	return (count);
}
