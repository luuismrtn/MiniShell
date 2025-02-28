/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/28 14:41:01:0 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void automata(t_token *tokens)
{
    int current_state;
    int prev_token;
    tokens = tokens->next;

    int automata[6][8] = {
        {1, 5, 5, 3, 3, 3, 3, 2}, //inicial
        {2, 1, 4, 3, 3, 3, 3, 1}, //comando
        {2, 2, 4, 3, 3, 3, 3, 2}, //file
        {2, 2, 5, 5, 5, 5, 5, 2}, //redireccion
        {1, 5, 5, 3, 3, 3, 3, 2}, //pipe
        {5, 5, 5, 5, 5, 5, 5, 5}  //err
    };// w  f  |  <  > <<  >> $

    current_state = 0;
    while(current_state != 5 && tokens != NULL)
    {
        prev_token = tokens->type;
        current_state = automata[current_state][tokens->type];
        tokens = tokens->next;
    }
    if (current_state != 1 && current_state != 2)
    {
        printf("Error: %d\n", prev_token);
        return;
    }
    else
        printf("Correcto\n");
}

