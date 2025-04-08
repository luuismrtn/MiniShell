/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:35:52 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/08 23:55:24 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Maneja la expansión de la variable HOME en la cadena de entrada
 *
 * Esta función maneja la expansión de "~" en la cadena de entrada.
 *
 * @param tokens Doble puntero a la lista de tokens donde se añadirán los nuevos
 * @param i Puntero al índice actual de lectura en la cadena de entrada
 */
void	handle_vir(t_token **tokens, char *input, int *i)
{
	t_env	*home;
	char	*home_content;

	if (!(input[*i + 1] == '/' || input[*i + 1] == '\0' || input[*i
				+ 1] == ' '))
	{
		(*i)++;
		add_token(tokens, T_WORD, ft_strdup("~"), 0);
		return ;
	}
	(*i)++;
	home = find_env_var((*tokens)->env_mshell, "HOME");
	home_content = home->content;
	if (!home_content)
		return ;
	add_token(tokens, T_WORD, ft_strdup(home_content), 0);
}
