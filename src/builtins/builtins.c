/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:21:08 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/12 16:54:47 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int count_last_char(char *str, char c)
{
	int i;
	int sol;

	sol = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			sol = i;
		i++;
	}
	return sol;
}

void modify_pwd(t_token **tokens, char *var, char *dir)
{
	t_env *aux;
	t_env *new_var;
	char *new_pwd;
	char *path;

	if (ft_strncmp(dir, ".", 2) == 0)
		return;
	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (ft_strncmp(aux->name, var, ft_strlen(var)) == 0)
		{
			if (ft_strncmp(dir, "..", 3) == 0)
			{
				new_pwd = ft_substr(aux->content, 0, count_last_char(aux->content, '/'));
				free(aux->content);
				aux->content = new_pwd;
				printf("modify pwd %s\n", aux->content);
			}
			else
			{
				path = ft_strjoin("/", dir);
				new_pwd = ft_strjoin(aux->content, path);
				free (path);
				free (aux->content);
				aux->content = new_pwd;
				printf("modify pwd %s\n", aux->content);
			}
			return;
		}
		aux = aux->next;
	}
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return;
	new_var->name = ft_strdup(var);
	path = ft_strjoin("/", dir);
	new_var->content = ft_strjoin(search_path(join_env((*tokens)->env_mshell), "HOME")[1], path);
	free (path);
	if (!new_var->content)
		return (free(new_var));
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}

void	handle_builtin(char **args, t_token *tokens)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		ft_echo(args);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		ft_cd(args, &tokens);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(args[0], "export", 7) == 0)
		ft_export(tokens, args);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		ft_unset(tokens, args);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		ft_env(tokens->env_mshell);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		ft_exit(args);
	else
		return ;
}