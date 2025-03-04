NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address 
RM = rm -f

SRC = src/main.c \
	src/pipex.c \
	src/history.c \
	src/tokenize.c \
	src/env_build_in.c \
	src/automata.c \
	src/exe.c \
	src/builtins.c \

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
# leaks con: echo hola
# despues de haber ejecutado algo, si se hace control d
# al ejecutar cada builtin
# echo $?

# TODOLIST
# history a veces guarda espacios o \n //sigue guardando repetidos a veces?? //puede estar a 0 cd se re-ejecuta
# make_command poner el env_mshell // env en el exe (y otros) (siempre usar env_mshell)
# gestionar quotes (con el tipo->hay que recuperarlo en el parseo) en make command --> "ls -a" command not found //split...
# segfault al hacer control-d despues de una ejecución
# guardar "shell level" y pwd al empezar -->en primer token cn el env
# señal control-c cd se ejecuta minishell dentro de minishell
# revisar si dejamos fd's abiertos
# echo "$USER" > output (problema expansión)
# lo del export; export e sale en export pero no en env (juego con otros niveles)
# ultimo espacio en tokens a veces problemas(relacion con hist?): fullcommand [0]ls, full command [1] ' ' ls: cannot access ''$'\n': No such file or directory

# $? ("$?" no funciona) (actualizar en los builtins ->(exit 300, echo $? da 44))
    #cambiado en padre despues de ejecutar (a 127) // control c (a 130)

# export
# env
# meter pipex --> implementar si detecta un builtin