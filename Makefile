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

#PROBLEMAS EXPANSION
# echo "$? $HOME"  --------->>>> el $?
# /home/luis-la /home/luis-la
# deberia dar --> 0 /home/luis-la
#      PERO esto sii ok: Input: echo "$USER $?"

#Input: echo "$ " ->> da cosas q no tiene q dar (quizas solo en MAC)


#------------------------------------------------------------------------OK 
#(lo dejo para mirar q siga funcionando cd este OK lo q queda)

#las normales solo pilla la ultima
# /home/aldferna/Desktop/MiniShell ~ echo "$USER $PWD"
# Token type: 0, content: echo
# Token type: 8, content:  
# Token type: 0, content: /home/aldferna/Desktop/MiniShell 
# /home/aldferna/Desktop/MiniShell ~ echo "$USER $USER"
# Token type: 0, content: echo
# Token type: 8, content:  
# Token type: 0, content: aldferna 

#mirar: echo "hola $USER que tal $USER"
#mirar: echo "hola $USER que tal $USER jeje"

#EN HEREDOC: solo expande la ultima
# C_Token type: 0, Content: cat
# C_Token type: 5, Content: <<
# C_Token type: 0, Content: h
# > $? $USER $PWD
# > h
# $? $USER /home/aldferna/Desktop/MiniShell