/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:30:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/24 10:43:51 by adrianafern      ###   ########.fr       */
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

static void	parse_cmd(char ***comnd)
{
	int		i;
	int		j;
	char	*first;
	char	**split_cmd;
	char	**new_cmd;

	if (!comnd || !*comnd || !(*comnd)[0])
		return ;
	first = ft_strdup((*comnd)[0]);
	if (!first)
		return ;
	i = 0;
	while (first[i] && !ft_isspace(first[i]))
		i++;
	if (!first[i])
		return (free(first));
	split_cmd = ft_split(first, ' ');
	if (!split_cmd)
		return (free(first));
	i = 0;
	while (split_cmd[i])
		i++;
	j = 0;
	while ((*comnd)[j])
		j++;
	new_cmd = malloc(sizeof(char *) * (i + j));
	i = -1;
	while (split_cmd[++i])
		new_cmd[i] = split_cmd[i];
	j = 1;
	while ((*comnd)[j])
		new_cmd[i++] = (*comnd)[j++];
	new_cmd[i] = NULL;
	free(first);
	free(split_cmd);
	free((*comnd)[0]);
	free(*comnd);
	*comnd = new_cmd;
}

void	exe(t_token *tokens, char **comnd, int stdout)
{
	char		**paths;
	int			i;
	struct stat	statbuf;

	parse_cmd(&comnd);
	print_2(comnd); //acordarse de quitar eso, q si no con algo tipo cat hola > 2, lo mete en 2
	if ((ft_strchr(comnd[0], '/') != NULL) && (access(comnd[0], X_OK) == 0))
	{
		close(stdout);
		execve(comnd[0], comnd, join_env(tokens->env_mshell));
	}
	else
	{
		paths = search_path(join_env(tokens->env_mshell), comnd[0]);
		i = 0;
		while (paths[i] != NULL)
		{
			if (access(paths[i], R_OK | X_OK) == 0)
			{
				close(stdout);
				execve(paths[i], comnd, join_env(tokens->env_mshell));
			}
			i++;
		}
		i = 0;
		while (paths[i])
			free(paths[i++]);
		free(paths);
	}
	dup2(stdout, STDOUT_FILENO);
	close(stdout);
	if (ft_strncmp(comnd[0], "./", 2) == 0)
	{
		if (stat(comnd[0], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
			// 0 si existe y pudo acceder
			printf("%s: Is a directory\n", comnd[0]);
		// la flag: deveulve true si es un dir
		else if (access(comnd[0], F_OK) != 0)
			printf("%s: No such file or directory\n", comnd[0]);
		exit_num = 126;
		exit(exit_num);
	}
	else
		printf("%s: command not found\n", comnd[0]);
	exit_num = 127;
	exit(exit_num);
}

char	**join_env(t_env *env_mshell)
{
	char	**env;
	t_env	*aux;
	int		i;
	char	*temp;

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
	aux = env_mshell;
	while (aux != NULL)
	{
		if (aux->content == NULL)
		{
			env[i] = ft_strjoin(aux->name, "");
		}
		else
		{
			temp = ft_strjoin(aux->name, "=");
			if (!temp)
			{
				while (--i >= 0)
					free(env[i]);
				free(env);
				return (NULL);
			}
			env[i] = ft_strjoin(temp, aux->content);
			free(temp);
			if (!env[i])
			{
				while (--i >= 0)
					free(env[i]);
				free(env);
				return (NULL);
			}
		}
		aux = aux->next;
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
