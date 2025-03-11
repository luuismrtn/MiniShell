/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:21:08 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/11 14:03:39 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h" //127 command not found

void	ft_echo(char **args)
{
	int	i;
	int	n;

	if (ft_strncmp(args[1], "-n", 3) == 0)
	{
		n = 1;
		i = 2;
	}
	else
	{
		n = 0;
		i = 1;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n)
		ft_putchar_fd('\n', 1);
}

void	ft_cd(char **args)
{
	char	*path;

	path = args[1];
	if (path == NULL)
	{
		path = getenv("HOME");
		if (path == NULL || chdir(path) != 0)
			perror("minishell");
	}
	else if (chdir(path) != 0)
		perror("minihsell");
}

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, 1);
	else
		perror("minishell");
}
void	ft_export(t_token *tokens, char **args)
{
	t_env	*current;
	t_env	*new_env;
	char	*var_name;
	char	*var_content;
	int		i;

	printf("export\n");
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			var_name = ft_substr(args[i], 0, ft_strchr(args[i], '=') - args[i]);
			var_content = ft_strchr(args[i], '=') + 1;
			current = tokens->env_mshell;
			while (current)
			{
				if (ft_strncmp(current->name, var_name,
						ft_strlen(var_name)) == 0)
				{
					free(current->content);
					current->content = ft_strdup(var_content);
					break ;
				}
				current = current->next;
			}
			if (!current)
			{
				new_env = malloc(sizeof(t_env));
				if (!new_env)
					return ;
				new_env->name = ft_strdup(var_name);
				new_env->content = ft_strdup(var_content);
				new_env->next = tokens->env_mshell;
				tokens->env_mshell = new_env;
			}
			free(var_name);
		}
		i++;
	}
}

void	ft_unset(t_token *tokens, char **args)
{
	t_env	*current;
	t_env	*prev;
	int		i;

	i = 1;
	while (args[i])
	{
		current = tokens->env_mshell;
		prev = NULL;
		while (current)
		{
			if (ft_strncmp(current->name, args[i], ft_strlen(args[i])) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					tokens->env_mshell = current->next;
				free(current->name);
				free(current->content);
				free(current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
}

void	ft_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_putstr_fd(current->name, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(current->content, 1);
		current = current->next;
	}
}

int arg_isdigit(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (!ft_isdigit(str[i]))
			return 0;
		i++;
	}
	return 1;
}

void	ft_exit(t_token *tokens, char **arg)
{
	int	i;
	(void)tokens;

	i = 0;
	while (arg[i])
		i++;
	if (i > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		exit_num = 1;
	}
	else if (arg[1])
	{
		exit_num = ft_atoi(arg[1]);
		if (arg_isdigit(arg[1]))
			exit (exit_num);
		else
		{
			write(2, "numeric argument required\n", 27);
			exit(2);
		}
	}
	else
		exit(0);
}

void	handle_builtin(char **args, t_token *tokens)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		ft_echo(args);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		ft_cd(args);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(args[0], "export", 7) == 0)
		ft_export(tokens, args);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		ft_unset(tokens, args);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		ft_env(tokens->env_mshell);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		ft_exit(tokens, args);
	else
		return ;
}