/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/27 23:49:01 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


int main(int argc, char **argv, char **env)
{
    char *line;
    (void)argc;
	(void)argv;

    if (ft_read_history(HISTORY_FILE) == ERROR)
        return ERROR;
    while (1)
    {
        line = readline("minishell ~ ");
        add_history(line);
        write_line_history(HISTORY_FILE, line);
        main2(line, env);
        rl_on_new_line();
        //clean_tokens
        //automat
    }
    return SUCCESS;
}
