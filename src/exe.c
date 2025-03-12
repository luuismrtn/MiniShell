/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:30:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 16:36:21 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	exe(t_token *tokens, char **comnd, int stdout)
{
	char	**paths;
	int		i;

	if ((ft_strchr(comnd[0], '/') != NULL) && (access(comnd[0], X_OK) == 0))
		execve(comnd[0], comnd, join_env(tokens->env_mshell));
	else
	{
		paths = search_path(join_env(tokens->env_mshell), comnd[0]);
		i = 0;
		while (paths[i] != NULL)
		{
			if (access(paths[i], R_OK | X_OK) == 0)
				execve(paths[i], comnd, join_env(tokens->env_mshell));
			i++;
		}
		i = 0;
		while (paths[i])
			free(paths[i++]);
		free(paths);
	}
	dup2(stdout, STDOUT_FILENO);
	close(stdout);
	printf("%s: command not found\n", comnd[0]); // perroor
	exit(127);
}

char	**join_env(t_env *env_mshell)
{
	char	**env;
	t_env	*aux;
	int		i;

	aux = env_mshell;
	i = 0;
	while (aux != NULL)
	{
		i++;
		aux = aux->next;
	}
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (env_mshell != NULL)
	{
		env[i] = ft_strjoin(env_mshell->name, "=");
		env[i] = ft_strjoin(env[i], env_mshell->content);
		env_mshell = env_mshell->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	is_builtin(char **cmnd)
{
	return ((ft_strncmp(cmnd[0], "echo", 5) == 0 || ft_strncmp(cmnd[0], "cd",
				3) == 0 || ft_strncmp(cmnd[0], "pwd", 4) == 0
			|| ft_strncmp(cmnd[0], "env", 4) == 0 || ft_strncmp(cmnd[0],
				"export", 7) == 0 || ft_strncmp(cmnd[0], "unset", 6) == 0
			|| ft_strncmp(cmnd[0], "exit", 5) == 0));
}
