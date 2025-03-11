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
# revisar si dejamos fd's abiertos
# echo $$ da un num
# el heredoc (+ meterlo tb en el pipex)
# $?: (actualizar en los builtins)
    #cambiado en padre despues de ejecutar (a 127) // control c (a 130) // (exit 300, echo $? da 44))

# clonado en un sitio q no sea HOME.. .hystory??
# si solo 'export' imprimir las variables exportadas

#bash-3.2$ echo < 1 | echo < 2 | echo < 3
#    bash: echo: write error: Broken pipe //meter un if en ecchooo??
# guardar $PWD (cuado haces cd) 
# cambiar getenv por nuestro getenv

