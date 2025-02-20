/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/20 16:15:40 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int main()
{
    char *line;

    if (read_history(HISTORY_FILE) == ERROR)
        return NULL;
    
    while (1)
    {
        line = readline("minishell ~ ");
        add_history(line);
        write_line_history(HISTORY_FILE, line);
        rl_on_new_line();
    }
    return 0;
}