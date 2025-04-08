/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:25:06 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/08 20:59:51 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Maneja las señales en el proceso principal del shell
 *
 * Esta función gestiona las señales SIGINT (Ctrl+C) y SIGQUIT (Ctrl+\)
 * en el proceso principal del shell. Para SIGINT, actualiza el código
 * de salida, muestra un nuevo prompt y reinicia la línea actual. Para
 * SIGQUIT, ignora la señal.
 *
 * @param sig Número de la señal recibida (SIGINT o SIGQUIT)
 */
void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_num = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		return ;
}

/**
 * @brief Maneja las señales en procesos hijos
 *
 * Esta función gestiona las señales SIGINT (Ctrl+C) y SIGQUIT (Ctrl+\)
 * en procesos hijos. Para SIGINT, actualiza el código de salida y
 * muestra una nueva línea. Para SIGQUIT, ignora la señal.
 *
 * @param sig Número de la señal recibida (SIGINT o SIGQUIT)
 */
void	handle_signal_child(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_num = 130;
		write(1, "\n", 1);
		rl_on_new_line();
	}
	else if (sig == SIGQUIT)
		return ;
}

/**
 * @brief Maneja las señales en procesos de heredoc
 *
 * Esta función gestiona las señales SIGINT (Ctrl+C) y SIGQUIT (Ctrl+\)
 * específicamente durante la lectura de un heredoc. Para SIGINT,
 * actualiza el código de salida, muestra una nueva línea y termina
 * el proceso de heredoc con código 130. Para SIGQUIT, ignora la señal.
 *
 * @param sig Número de la señal recibida (SIGINT o SIGQUIT)
 */
void	handle_signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_num = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		exit(130);
	}
	else if (sig == SIGQUIT)
		return ;
}

/**
 * @brief Configura los manejadores de señales según el contexto
 *
 * Esta función configura SIGINT y SIGQUIT con diferentes manejadores
 * según el contexto de ejecución especificado por el parámetro:
 * - 'f': Configuración para el shell principal
 * - 'c': Configuración para procesos hijos
 * - 'h': Configuración para procesos de heredoc
 *
 * @param c Carácter que indica el contexto ('f', 'c' o 'h')
 */
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

/**
 * @brief Ignora completamente las señales SIGINT y SIGQUIT
 *
 * Esta función configura tanto SIGINT como SIGQUIT para ser ignoradas.
 */
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
