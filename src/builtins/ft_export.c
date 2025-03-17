/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:16:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/17 18:58:11 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_env_as_export(t_env *env)
{
	while (env)
	{
		if (env->content)
			printf("declare -x %s=\"%s\"\n", env->name, env->content);
		else
			printf("declare -x %s\n", env->name);
		env = env->next;
	}
}

static t_env	*find_env_var(t_env *env, char *var_name)
{
	while (env)
	{
		if (ft_strncmp(env->name, var_name, ft_strlen(var_name)) == 0
			&& ft_strlen(env->name) == ft_strlen(var_name))
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	add_env_var(t_token *tokens, char *name, char *content)
{
	t_env	*new_env;
	char *to_search;
	t_env *current;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->name = ft_strdup(name);
	if (content[0] == '$')
	{
		to_search = ft_substr(content, 1, ft_strlen(content) - 1);
		current = find_env_var(tokens->env_mshell, to_search);
		free(to_search);
		new_env->content = ft_strdup(current->content);
	}
	else if(content)
		new_env->content = ft_strdup(content);
	else
		new_env->content = NULL;
	new_env->next = tokens->env_mshell;
	tokens->env_mshell = new_env;
}

static void	process_assignment(t_token *tokens, char *arg)
{
	char	*var_name;
	char	*var_content;
	t_env	*current;

	var_name = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
	var_content = ft_strchr(arg, '=') + 1;
	current = find_env_var(tokens->env_mshell, var_name);
	if (current)
	{
		free(current->content);
		current->content = ft_strdup(var_content);
	}
	else
		add_env_var(tokens, var_name, var_content);
	free(var_name);
}

static void	process_declaration(t_token *tokens, char *arg)
{
	t_env	*current;

	current = find_env_var(tokens->env_mshell, arg);
	if (!current)
		add_env_var(tokens, arg, NULL);
}

static int	is_valid_var_name(char *name)
{
	int	i;

	if (!name || name[0] == '\0')
		return (0);
	i = 0;
	while (name[i])
	{
		if (!(ft_isalpha(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static char	*extract_var_name(char *arg)
{
	char	*equal_pos;
	char	*var_name;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		var_name = ft_substr(arg, 0, equal_pos - arg);
	else
		var_name = ft_strdup(arg);
	return (var_name);
}

static int	check_args(char **args)
{
	int		i;
	char	*var_name;
	int		result;

	i = 1;
	result = SUCCESS;
	while (args[i])
	{
		var_name = extract_var_name(args[i]);
		if (!is_valid_var_name(var_name))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_num = 1;
			result = ERROR;
		}
		free(var_name);
		i++;
	}
	return (result);
}

void	ft_export(t_token *tokens, char **args)
{
	int		i;
	char	*var_name;

	if (!args[1])
	{
		print_env_as_export(tokens->env_mshell);
		return ;
	}
	if (check_args(args) == ERROR)
		return ;
	i = 1;
	while (args[i])
	{
		var_name = extract_var_name(args[i]);
		if (is_valid_var_name(var_name))
		{
			if (ft_strchr(args[i], '='))
				process_assignment(tokens, args[i]);
			else
				process_declaration(tokens, args[i]);
		}
		free(var_name);
		i++;
	}
}
