/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:24:32 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/25 15:24:55 by aldferna         ###   ########.fr       */
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
char					*get_path_from_env(char **env);
void					try_exec_with_path(char *cmd, char **args, char **env);
int						count_env_vars(t_env *env_list);

//  PIPEX
void					pipex(t_token *tokens, int num_commands);
void					setup_redirections(t_token *tokens, int (*fds)[2],
							int count);
char					**build_command_string(t_token *tokens, int count);
void					executor(t_token **tokens, int (*fds)[2], char **args,
							int original_stdout);
void					one_comnd(t_token **tokens);

//	PIPEX UTILS
int						child_pipe_fdin_redir(int *fd_in, char **args,
							int (*connect)[2]);
void					change_fds_redir(int (*fds)[2], int *o_stdin,
							int *o_stdout, int builtin_father);
void					clean_father_material(int (*fds)[2], char ***args);
void					errors_pipex(int *pipe_in, int *pipe_out, char ***args,
							char c);

//  BUILTINS
void					handle_builtin(char **args, t_token *tokens);
void					modify_pwd(t_token **tokens, char *dir);
void					modify_shlvl(t_token **tokens, char *var);
void					ft_cd(char **args, t_token **tokens);
void					ft_echo(char **args);
void					ft_env(t_env *env);
void					ft_exit(char **arg);
void					ft_export(t_token *tokens, char **args);
void					ft_pwd(t_token *tokens);
void					ft_unset(t_token *tokens, char **args);

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
int						match_string(char *str1, char *str2);
char					*handle_env_var(char *str, t_token *tokens);
int						num_pipes(char *str);
int						len_array(char **array);
int						count_args(char **args);

// LIST UTILS
int						has_pipe(t_token *tokens);

#endif
