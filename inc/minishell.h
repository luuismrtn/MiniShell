/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/20 13:01:03 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern unsigned char	exit_num;

# define SUCCESS 0
# define ERROR 1

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
	t_env				*exp_var;
	t_token_value		type;
	char				*content;
	int					quotes;
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
t_token					*tokenize(char *input, t_token *tokens);
void					delete_tokens(t_token **tokens);
void					clean_tokens(t_token **tokens);
int						automata(t_token *tokens);
int						ft_len_var_name(char *str, int i);

// ENV
t_env					*env_buildin(char **env);
t_env					*find_env_var(t_env *env, char *var_name);

// EXE
char					**join_env(t_env *env_mshell);
void					exe(t_token *tokens, char **comnd, int stdout);
char					**search_path(char **env, char *comnd);
int						is_builtin(char **cmnd);

//  PIPEX
int						pipex(char *argv, t_token *tokens);
void					setup_redirections(t_token *tokens, int (*fds)[2],
							int count);
char					**build_command_string(t_token *tokens, int *count);

//  BUILTINS
void					handle_builtin(char **args, t_token *tokens);
void					modify_pwd(t_token **tokens, char *dir);
void					ft_cd(char **args, t_token **tokens);
void					ft_echo(char **args);
void					ft_env(t_env *env);
void					ft_exit(char **arg);
void					ft_export(t_token *tokens, char **args);
void					ft_pwd(t_token *tokens);
void					ft_unset(t_token *tokens, char **args);
void					modify_pwd(t_token **tokens, char *dir);

// CD_UTILS
void					print_cd_error(char *path);
void					handle_broken_pwd(t_token **tokens, char *input_path);
int						validate_input_cd(char *input);
char					*find_path(char **args);

// EXPORT_UTILS
int						is_valid_var_name(char *name);
char					*extract_var_name(char *arg);
void					print_env_as_export(t_token *tokens);
void					handle_add_var(t_token *tokens, t_env *new_env,
							char *content);

// MODIFY_PWD_UTILS
void					create_new_pwd(t_token **tokens, char *dir);

//	SIGNALS
void					signals(char c);
void					ign_signal(void);

int						main2(char *string, t_token *tokens);

void					print_2(char **str);
void					free_array(char **array);
void					handle_signal_heredoc(int sig);

// UTILS
void					get_env_content_and_replace(t_token **tokens,
							char *name, char *content);
int						count_args(char **args);
int						match_string(char *str1, char *str2);
char					*handle_env_var(char *str, t_token *tokens);

#endif
