NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g3 #-fsanitize=address
RM = rm -f

SRC = src/main.c \
	src/exe/executor_pipex.c src/exe/executor.c src/exe/executor_utils.c\
	src/exe/exe.c src/exe/env_exe.c src/exe/search_path_exe.c \
	src/history.c \
	src/tokenize.c \
	src/env_build_in.c \
	src/automata.c \
	src/builtins/builtins.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_cd_utils.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_env.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_export_utils.c \
	src/builtins/ft_pwd.c \
	src/builtins/ft_unset.c \
	src/builtins/modify_pwd.c \
	src/builtins/modify_pwd_utils.c \
	src/modify_env.c \
	src/utils_general.c\

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
# /home/aldferna/Desktop/MiniShell$
# /home/aldferna/Desktop/MiniShell ~ (control D)

# echo hola | ls

# jdf | sleep 2





#mirar fds abiertos





# MASS PRUEBAS CON ERROR:
#seguir mirando lo de $p="ls -l"
#"ls -l" no deberiaa funcionar

# /home/aldferna/Desktop/MiniShell ~ ./minishell | ./minishell
# /home/aldferna/Desktop/MiniShell ~ /home/aldferna/Desktop/MiniShell ~ 

# /home/aldferna/Desktop/MiniShell ~ cat | cat | ls
# errors.txt  inc  libft  Makefile  minishell  src
# ^C
# /home/aldferna/Desktop/MiniShell ~ /home/aldferna/Desktop/MiniShell ~ 




#--------------------cosas de las q podriamos pasar:

#echo $"$'$PWD'"  (bash: $'/home/aldferna/Desktop/MiniShell') (mshell: $$'/home/aldferna/Desktop/MiniShell')
