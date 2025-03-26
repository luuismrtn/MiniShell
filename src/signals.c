/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:25:06 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/26 20:25:32 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
		write(1, "\n", 1);
		//rl_replace_line("", 0);
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
		//rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (sig == SIGQUIT)
		return ;
}

void	handle_signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		exit_num = 130;
		write(1, "\n", 1);
		//rl_replace_line("", 0);
		rl_on_new_line();
		exit(130);
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
	else if (c == 'h')
		sa.sa_handler = handle_signal_heredoc;
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
