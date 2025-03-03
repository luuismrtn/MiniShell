/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:21:08 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/03 19:23:42 by aldferna         ###   ########.fr       */
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
    char *path;

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
/*
void	ft_export(char *var)
{
}


void	ft_unset(char *var)
{
}
*/

void	ft_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
}

void	ft_exit(void)
{
	return ;
}

void	handle_builtin(char **args, char **env)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		ft_echo(args);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		ft_cd(args);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		ft_pwd();
	/*
else if	(ft_strncmp(args[0], "export", 7) == 0)
	ft_export(args[1]);
else if	(ft_strncmp(args[0], "unset", 6) == 0)
	ft_unset(args[1]);
	*/
	else if (ft_strncmp(args[0], "env", 4) == 0)
		ft_env(env);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		ft_exit();
	else
		return ;
}