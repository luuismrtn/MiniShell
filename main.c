/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/20 15:54:35 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main()
{
    char *line;
    
    while (1)
    {
        line = readline("minishell ~ ");
        add_history(line);

        rl_on_new_line();
    }
    return 0;
}