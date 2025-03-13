/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/13 21:10:18 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char	exit_num = 0;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		return ;
}

void	handle_signal_child(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (sig == SIGQUIT)
		return ;
}

void	signals(char c)
{
	struct sigaction	sa;

	if (c == 'f')
		sa.sa_handler = handle_signal;
	else if (c == 'c')
		sa.sa_handler = handle_signal_child;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction1 \n");
		exit(1);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction2 \n");
		exit(1);
	}
}

void	ign_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction1 \n");
		exit(1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction2 \n");
		exit(1);
	}
}

void	print_2(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("str[%d] = %s\n", i, str[i]);
		i++;
	}
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*get_history_path(void)
{
	char	*path;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	path = ft_strjoin(cwd, "/.minishell_history");
	return (path);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_token	*tokens;
	char	*HISTORY_FILE;

	HISTORY_FILE = get_history_path();
	(void)argc;
	(void)argv;
	tokens = malloc(sizeof(t_token));
	ft_memset(tokens, 0, sizeof(t_token));
	tokens->env_mshell = env_buildin(env);
	if (ft_read_history(HISTORY_FILE) == ERROR)
		return (ERROR);
	signals('f');
	tokens->content = ft_strdup("0");
	while (1)
	{

		line = readline(ft_strjoin(get_env_content(tokens->env_mshell, "PWD"), " ~ "));
		if (!line)
			break ;
		if (line[0] == '\0' || ft_strtrim(line, " \t\n\r\f\v")[0] == '\0')
		{
			free(line);
			continue ;
		}
		else
		{
			add_history(line);
			write_line_history(HISTORY_FILE, line);
			main2(line, tokens);
			free(line);
		}
		delete_tokens(&tokens);
	}
	return (SUCCESS);
}
