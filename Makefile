NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g3 #-fsanitize=address
RM = rm -f

SRC = src/main.c \
	src/history.c \
	src/env_build_in.c \
	src/signals.c \
	src/tokenize/tokenize.c \
	src/tokenize/clean_tokens.c \
	src/tokenize/handle_quotes.c \
	src/tokenize/handle_quotes_utils.c \
	src/tokenize/handle_redirections.c \
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
	src/utils_general.c\
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


# LEAKS:
#pwd | cat | cat -e >3 | wc -l

# /home/aldferna/Desktop/MiniShell ~ cat | cat | ls
# 3  errors.txt  inc  libft  Makefile  minishell  src
# /home/aldferna/Desktop/MiniShell ~ ^C
# /home/aldferna/Desktop/MiniShell ~  (control D)

# /home/aldferna/Desktop/MiniShell ~ cat 3
#	 /home/aldferna/Desktop/MiniShell$
# /home/aldferna/Desktop/MiniShell ~ (control D)

# echo hola | ls

# jdf | sleep 2





#mirar fds abiertos





# MASS PRUEBAS CON ERROR:

# /home/aldferna/Desktop/MiniShell ~ echo $user
# [1]    200774 segmentation fault (core dumped)  ./minishell

# /home/aldferna/Desktop/MiniShell ~ << eof | ls -l
# ls: write error: Bad file descriptor

#pueden hacer: env -i ./minishell
#en ese caso: leer /etc/environment (guardas path desde este y añades PWD y SHLVL->si no exite: 1)
#si luego quitan ademas luego hacen env -i $PATH; minishell no ejecuta comandos pero no debe petar

# /home/aldferna/Desktop/MiniShell ~ ./minishell | ./minishell
# /home/aldferna/Desktop/MiniShell ~ /home/aldferna/Desktop/MiniShell ~ 
#esto antes funcionaba (no se cd), puede ser q tenga relacion con q no se crea un hijo

#con cd -
# ➜  MiniShell git:(main) ✗ make && ./minishell
# /home/aldferna/Desktop/MiniShell ~ cd
# /home/aldferna ~ ^C
# /home/aldferna ~ ^C
# /home/aldferna ~ cd -
# double free or corruption (fasttop)
# [1]    2844121 IOT instruction (core dumped)  ./minishell

# ➜  MiniShell git:(main) ✗ make && ./minishell
# /home/aldferna/Desktop/MiniShell ~ cd -
# �m ~ 

#limitar historial (uede petar si esta muy lleno)-> al momento de escribir en el archivo; cuantass lineas hay?


#--------------------cosas de las q podriamos pasar:

#echo $"$'$PWD'"  (bash: $'/home/aldferna/Desktop/MiniShell') (mshell: $$'/home/aldferna/Desktop/MiniShell')
