/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:09 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/20 18:26:17 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(void)
{
	char	*line;

	if (ft_read_history(HISTORY_FILE) == ERROR)
		return (ERROR);
	while (1)
	{
		line = readline("minishell ~ ");
		write_line_history(HISTORY_FILE, line);
		rl_on_new_line();
	}
	return (SUCCESS);
}
