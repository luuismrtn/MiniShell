/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   question_mark.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:11:34 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/11 00:39:15 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	change_question_mark(t_token *tokens, int status)
{
	tokens->content = ft_itoa(status);
}

int	get_question_mark(t_token *tokens)
{
	return (ft_atoi(tokens->content));
}
