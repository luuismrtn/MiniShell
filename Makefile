NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g3 #-fsanitize=address
RM = rm -f

SRC = src/main.c \
	src/pipex.c \
	src/history.c \
	src/tokenize.c \
	src/env_build_in.c \
	src/automata.c \
	src/exe.c \
	src/builtins/builtins.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_env.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_pwd.c \
	src/builtins/ft_unset.c \

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


# LEAKS
# quotes not closed -> leak + espacio final en clean_tokens
# leaks con: hola... "no such file or directory"
# con: ñ
# despues de haber ejecutado algo, si se hace control d
# al ejecutar cada builtin
# leaks con: echo hola
# echo $?

# TODOLIST
# revisar si dejamos fd's abiertos
# el heredoc: expansion: boleano comillas/no comillas en el delimitador




# echo "$? $HOME"
# /home/luis-la /home/luis-la
# deberia dar --> 0 /home/luis-la

#---------------------------------------------------------------------
#escribe siempre que se hace cd (OKK)
#/home/aldferna/Desktop/MiniShell ~ cd
#/home/aldferna/Desktop/MiniShell//home/aldferna ~ cd
#/home/aldferna/Desktop/MiniShell//home/aldferna//home/aldferna ~ cd


#borra una de mas cd ../../../  (OKK, guay estoooo, solo el >=)
#/home/aldferna/Desktop/MiniShell/1/2/3 ~ cd ../../../
#/home/aldferna/Desktop ~ ls
#1  inc  libft  Makefile  minishell  src


# cd .. hasta que si existe, entonces actualiza el pwd (no sigue añadiendo ../, ni da error)
#/home/aldferna/Desktop/MiniShell/1/2/3/.../../../../../../../../../../../.. ~ cd
#/home/aldferna/Desktop/MiniShell/1/2/3/.../../../../../../../../../../../..//home/aldferna ~ cd
#/home/aldferna/Desktop/MiniShell/1/2/3/.../../../../../../../../../../../..//home/aldferna//home/aldferna ~ pwd
#/home/aldferna

#error si mas de dos puntoooos:
#/home/aldferna/Desktop/MiniShell/1/2/3/.../../
