/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:09:26 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/05 18:47:29 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
