NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g3 -fsanitize=address
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
# despues de haber ejecutado algo, si se hace control d
# al ejecutar cada builtin
# leaks con: echo hola
# echo $?

# TODOLIST
# guardar "shell level" y pwd al empezar -->en primer token cn el env
# señal control-c cd se ejecuta minishell dentro de minishell
# revisar si dejamos fd's abiertos
# eco $$ da un num
# el heredoc (+ meterlo tb en el pipex)
# exit: mal lo de devolver 1 si too many args (y no cerrar) //da segfault
# $?: (actualizar en los builtins)
    #cambiado en padre despues de ejecutar (a 127) // control c (a 130) // (exit 300, echo $? da 44))

# clonado en un sitio q no sea HOME.. .hystory??
# decidir q hacer si ponen solo 'export' --> imprimir env+var exportadas o nada???
# first_command: un if si solo es un comando, o se puede hacer en el ultimo if y luego cerrar la pipe?