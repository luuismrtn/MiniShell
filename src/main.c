/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/03 19:50:32 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char exit_num = 0;

void handle_signal(int sig)
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

void signals()
{
    struct sigaction sa;
    
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    //sigemptyser(&sa.sa_mask); //limpiar señales bloqueadas
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

int main(int argc, char **argv, char **env)
{
    char *line;
    (void)argc;
	(void)argv;
    
    //lo primero, antes de recibir cualquier cosa=> guadar env
    if (ft_read_history(HISTORY_FILE) == ERROR)
        return ERROR;
    signals();
    while (1)
    {
        line = readline("minishell ~ ");
        if (!line)
            break;
        if (line[0] == '\0')
        {
            rl_replace_line("", 0);
            rl_on_new_line();
            rl_redisplay();
        }
        else 
        {
            add_history(line);
            write_line_history(HISTORY_FILE, line);
            main2(line, env);
            rl_on_new_line();
        }
    }
    return SUCCESS;
}
