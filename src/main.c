/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/28 12:30:59 by aldferna         ###   ########.fr       */
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
    }
    return SUCCESS;
}
