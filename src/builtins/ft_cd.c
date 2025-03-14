/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:14:54 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/14 17:59:37 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_pwd_back(char *dir, char *pwd)
{
	char	**components;
	char	*result;
	int		levels_up;
	int		i;

	if (getcwd(NULL, 0) == NULL)
		return ft_strjoin(pwd, ft_strjoin("/", dir));
	levels_up = 0;
	components = ft_split(dir, '/');
	i = 0;
	while (components && components[i])
	{
		if (ft_strncmp(components[i], "..", 3) == 0)
			levels_up++;
		i++;
	}
	free_array(components);
	i = ft_strlen(pwd);
	while (i > 0 && levels_up > 0)
	{
		if (pwd[i] == '/')
			levels_up--;
		i--;
	}
	result = ft_substr(pwd, 0, i + 1);
	return (result);
}

void	modify_pwd(t_token **tokens, char *dir)
{
	t_env	*aux;
	t_env	*new_var;
	char	*new_pwd;
	char	*path;

	if (ft_strncmp(dir, ".", 2) == 0)
		return ;
	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (ft_strncmp(aux->name, "PWD", ft_strlen("PWD")) == 0)
		{
			if (ft_strnstr(dir, "..", 2)) //aqui paraa los de ... ?
			{
				new_pwd = handle_pwd_back(dir, aux->content);
				free(aux->content);
				aux->content = new_pwd;
			}
			else if (!ft_strncmp(dir, get_env_content((*tokens)->env_mshell, "HOME"), 5))
			{
				new_pwd = get_env_content((*tokens)->env_mshell, "HOME");
				free(aux->content);
				aux->content = new_pwd;
			}
			else
			{
				path = ft_strjoin("/", dir);
				new_pwd = ft_strjoin(aux->content, path);
				free(path);
				free(aux->content);
				aux->content = new_pwd;
			}
			return ;
		}
		aux = aux->next;
	}
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdup("PWD");
	path = ft_strjoin("/", dir);
	new_var->content = ft_strjoin(search_path(join_env((*tokens)->env_mshell),
				"HOME")[1], path);
	free(path);
	if (!new_var->content)
		return (free(new_var));
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}

static void	cd_to_home(t_token **tokens)
{
	char	*home_path;
	char 	*actual_path;

	home_path = get_env_content((*tokens)->env_mshell, "HOME");
	if (!home_path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return ;
	}
	actual_path = get_env_content((*tokens)->env_mshell, "PWD");
	if (ft_strncmp(home_path, actual_path, ft_strlen(actual_path)))
	{
		if (chdir(home_path) == 0)
			modify_pwd(tokens, home_path);	
	}
	// else
	// {
	// 	ft_putstr_fd("cd: ", 2);
	// 	ft_putstr_fd(home_path, 2);
	// 	ft_putstr_fd(": No such file or directory\n", 2);
	// }
}

static char	*find_path(char **args)
{
	char	*path;

	if (args[1][ft_strlen(args[1]) - 1] == '/')
		path = ft_substr(args[1], 0, ft_strlen(args[1]) - 1);
	else
		path = ft_strdup(args[1]);
	return (path);
}

char *find_desired_path(char *pwd, char *dir)
{
	char	**components_dir;
	char	**components_pwd;
	char	*result;
	int		levels_up;
	int		i;

	levels_up = 0;
	components_dir = ft_split(dir, '/');
	components_pwd = ft_split(pwd, '/');
	i = 0;
	while (components_dir && components_dir[i])
	{
		if (ft_strncmp(components_dir[i], "..", 3) == 0)
			levels_up++;
		i++;
	}
	i = 0;
	while (components_pwd && components_pwd[i])
	{
		if (ft_strncmp(components_pwd[i], "..", 3) == 0)
			levels_up++;
		i++;
	}
	free_array(components_dir);
	free_array(components_pwd);
	i = ft_strlen(pwd);
	levels_up = (levels_up * 2) - 1;
	while (i > 0 && levels_up > 0)
	{
		if (pwd[i] == '/')
			levels_up--;
		i--;
	}
	result = ft_substr(pwd, 0, i + 1);
	return (result);
}

static int validate_input(char *input)
{
	char **chop_input;
	int i;
	int ok;

	i = 0;
	ok = 0;
	chop_input = ft_split(input, '/');
	while (chop_input[i])
	{
		if (ft_strncmp(chop_input[i], "..", 3) == 0)
			ok++;
		i++;
	}
	free_array(chop_input);
	if (ok == i)
		return 1;
	return 0;
}

static void	execute_cd(char *input_path, t_token **tokens)
{
	char *desired_path;

	if (getcwd(NULL, 0) == NULL)
	{
		if (!validate_input(input_path))
		{
			write(2, "not such file or directory\n", 28);
			exit_num = 1;
			return;
		}
		desired_path = find_desired_path(get_env_content((*tokens)->env_mshell, "PWD"), input_path);
		if (chdir(desired_path) == 0)
		{
			get_env_content_and_replace(tokens, "PWD", desired_path);
			//return; //quitad ahora
		}
		else
		{
			printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			modify_pwd(tokens, input_path);
		}
	}
	else
	{
		if (chdir(input_path) == 0)
			modify_pwd(tokens, input_path);
		else
		{
			exit_num = 1;
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(input_path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}	
}

void	ft_cd(char **args, t_token **tokens)
{
	char	*path;

	if (!args[1])
		cd_to_home(tokens);
	else if (args[1] && args[2])
	{
		exit_num = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	else
	{
		path = find_path(args);
		execute_cd(path, tokens);
	}
}
