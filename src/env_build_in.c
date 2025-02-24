/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_build_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:01:35 by aldferna          #+#    #+#             */
/*   Updated: 2025/02/24 18:49:55 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void add_last(t_env **head, t_env *node) //alguna forma de generalizar esta funcion
{
    t_env *aux;
    
    if (*head == NULL)
        *head = node;
    else
    {
        aux = *head;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = node;
    }
}

t_env *env_buildin(char **env)
{
    t_env *env_mshell = NULL;
    t_env *node;
    char *aux;
    int i;
    int x;
    
    i = 0;
    while (env[i])
    {
        x = 0;
        while (env[i][x] && env[i][x] != '=')
            x++;
        node = malloc(sizeof(t_env));
        if (!node)
            return NULL;
        aux = ft_substr(env[i], 0, x);
        if (!aux)
            return (free(node), NULL);
        node->name = aux;
        x++;
        aux = ft_substr(env[i], x, ft_strlen(env[i]) - x);
        if (!aux)
            return (free(node->name), free(node), NULL);
        node->content = aux;
        node->next = NULL;
        add_last(&env_mshell, node);
        i++;
    }
    return (env_mshell);
}
