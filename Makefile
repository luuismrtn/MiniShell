NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=address
RM = rm -f

SRC = src/main.c \
	src/history.c \
	src/history_utils.c \
	src/env_build_in.c \
	src/signals.c \
	src/tokenize/tokenize.c \
	src/tokenize/clean_tokens.c \
	src/tokenize/handle_quotes.c \
	src/tokenize/handle_quotes_utils.c \
	src/tokenize/handle_redirections.c \
	src/tokenize/handle_vir.c \
	src/tokenize/tokens.c \
	src/automata.c \
	src/setup_redir.c \
	src/heredoc.c \
	src/heredoc_utils.c \
	src/build_command.c \
	src/exe/executor_pipex.c \
	src/exe/executor.c \
	src/exe/executor_utils.c \
	src/exe/exe.c \
	src/exe/env_exe.c \
	src/exe/search_path_exe.c \
	src/builtins/builtins.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_cd_utils.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_env.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_export_utils.c \
	src/builtins/ft_export_utils_2.c \
	src/builtins/ft_pwd.c \
	src/builtins/ft_unset.c \
	src/builtins/modify_pwd.c \
	src/builtins/modify_pwd_utils.c \
	src/modify_env.c \
	src/utils_general.c \
	src/utils_general_2.c \
	src/utils_errors.c \

INCLUDE = -I libft -I.

LIB = libft/libft.a

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -lreadline -o $(NAME)

clean:
	$(RM) $(OBJ)
	make clean -C libft

fclean: clean
	$(RM) $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
