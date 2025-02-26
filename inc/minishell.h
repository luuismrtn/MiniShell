/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/26 16:58:23 by aldferna         ###   ########.fr       */
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
	T_WORD,
	T_SPACE,
	T_HERE_DOC,
	T_REDIR_LEFT,
	T_APPEND,
	T_REDIR_RIGHT,
	T_PIPE,
	T_S_QUOTE,
	T_D_QUOTE,
	T_ENV
}					t_token_value;

typedef struct s_env
{
	char			*name;
	char			*content;
	struct s_env	*next;
}					t_env;

typedef struct s_token
{
	t_env *env_mshell;
	t_token_value	type;
	char			*content;
	char			*args;
	// char			*expanded; creo q guardar ej: "$USER" y "aldferna" no hace falta
	struct s_token	*next;
}					t_token;

typedef struct s_result
{
	char			*content;
	int				len;
}					t_result;

//  PIPEX
int					pipex(int argc, char **argv, char **env);

//  HISTORY
int					ft_read_history(char *history_file);
int					write_line_history(char *history_file, char *line);

//  PARSE
int					check_quotes(char *input);
t_token				*tokenize(char *line, char **env);
// clean_tokens -- juntar si no hay espacios: ca"t" + quitar espacios
// automata
// automata_functions

// ENV
t_env				*env_buildin(char **env);

#endif

// el len
// clean tokens