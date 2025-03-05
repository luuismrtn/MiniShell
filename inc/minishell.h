/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/05 20:22:06 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "pipex.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

extern unsigned char	exit_num;

# define SUCCESS 0
# define ERROR 1

# define HISTORY_FILE ".minishell_history"

typedef enum s_token_value
{
	T_WORD,
	T_FLAG,
	T_PIPE,
	T_REDIR_LEFT,
	T_REDIR_RIGHT,
	T_HERE_DOC,
	T_APPEND,
	T_ENV,
	T_SPACE,
	T_S_QUOTE,
	T_D_QUOTE
}						t_token_value;

typedef struct s_env
{
	char				*name;
	char				*content;
	struct s_env		*next;
}						t_env;

typedef struct s_token
{
	t_env				*env_mshell;
	t_token_value		type;
	char				*content;
	char				*args;
	struct s_token		*next;
}						t_token;

typedef struct s_result
{
	char				*content;
	int					len;
}						t_result;

//  HISTORY
int						ft_read_history(char *history_file);
int						write_line_history(char *history_file, char *line);

//  PARSE
int						check_quotes_closed(char *input);
t_token					*tokenize(char *line, char **env);
void					clean_tokens(t_token **tokens);
int						automata(t_token *tokens);

// ENV
t_env					*env_buildin(char **env);

// EXE
void					make_exe_command(t_token *tokens, char **env);
void					exe(char **env, char **comnd);
char					**search_path(char **env, char *comnd);

//  PIPEX
int						pipex(int argc, char **argv, char **env);
void	setup_redirections(t_token *tokens, int (*fds)[2], int num_comd, int *count);
char	*build_command_string(t_token *tokens,int num_comnd, int *count);

//  BUILTINS
void					handle_builtin(char **args, char **env);
void	ft_exit(char **arg);

int						main2(char *string, char **env);

#endif
