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
# leaks con: echo hola
# despues de haber ejecutado algo, si se hace control d
# al ejecutar cada builtin
# echo $?

# TODOLIST
# guardar "shell level" y pwd al empezar -->en primer token cn el env
# señal control-c cd se ejecuta minishell dentro de minishell
# revisar si dejamos fd's abiertos
# eco $$ da un num
# el heredoc (+ meterlo tb en el pipex)
# gestionar quotes (con el tipo->hay que recuperarlo en el parseo) en make command --> "ls -a" command not found //split...
# exit: mal lo de devolver 1 si too many args (y no cerrar) //da segfault
# $?: (actualizar en los builtins)
    #cambiado en padre despues de ejecutar (a 127) // control c (a 130) // (exit 300, echo $? da 44))