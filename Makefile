NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address 
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
# leaks con: echo hola
# despues de haber ejecutado algo, si se hace control d

# TODOLIST
# cosas raras history
# make_command poner el env_mshell // env en el exe (y otros) (?? cd usar env / env_mshell)
# gestionar quotes (con el tipo) en make command --> "ls -a" command not found //split...
# segfault al hacer control-d
# guardar "shell level" y pwd al empezar

# señales
# $? --> variable global (mirar que "$?" tb funcione)
# export
# env