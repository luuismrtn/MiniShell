/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/21 20:58:10 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "pipex.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

# define HISTORY_FILE ".minishell_history"

typedef enum s_token_value
{
	WORD,
	IS_SPACE,
	HERE_DOC,
	REDIR_LEFT,
	APPEND,
	REDIR_RIGHT,
	PIPE,
	S_QUOTE,
	D_QUOTE,
	ENV
}					t_token_value;

typedef struct s_token
{
	t_token_value	type;
	char			*content;
	struct s_token	*next;
}					t_token;

//  PIPEX
int					pipex(int argc, char **argv, char **env);

//  HISTORY
int					ft_read_history(char *history_file);
int					write_line_history(char *history_file, char *line);

//  PARSE
t_token				*tokenize(char *line);
//clean_tokens -- juntar si no hay espacios: ca"t" + quitar espacios
//automata
//automata_functions

#endif