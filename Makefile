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
	src/builtins/modify_pwd.c

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

# MASS PRUEBAS CON ERROR

# aldferna@c2r9s3:~/Desktop/MiniShell$ cat << l'e't
# > $HOME
# > let
# $HOME

# /home/aldferna/Desktop/MiniShell ~ cat << $HOME
# > hola
# > $HOME
# hola
# /home/aldferna

#export a
#./minishell
#se queda como sin env

#exit ajkjga asjdgdakjfv
#nos da too many arguments pero tiene que ser numeric argument required, con exit 2 (y si qu sale)
#solo too many si el 1º es numerico y hay mas argumentos numericos o no

# /home/aldferna/Desktop/MiniShell ~ << $A   (no para, probablemente expandde el eof)
# > hola
# > hola
# > $A
# > $A
# > 
# /home/aldferna/Desktop/MiniShell ~ cat << $A (heap use after free)
# > $A

#heredoc control c tiene que salir

# echo $a?$PATH
# echo $a+++++$PATH
# $a+++++$PATH -------estas tres anteriores, tiene que expandir ambas (si estan separadas por no:alfanum)
# echo $PATHa$HOME ----pero esta solo expande HOME, la anterior la toma como PATHa y no la encuentra (esto ahora ok, pero cuidado cd implementemos lo de arriba)
# echo $$$PATH ----cuidado con esto tb cd se implemente lo de arriba
# echo $1234$HOME (bash: 234/home/aldferna), (mshell: /home/aldferna)
# echo $1234-$HOME (bash: 234-/home/aldferna), (mshell: -/home/aldferna)

#implementar: (son args no flags)
#cd -
#cd ~
#cd //
#cd --
#cd este implementado probar si hay un dir '-'

#cd ../aldferna/..  (esto ahora funciona, pero lo dejo para probar cd el resto tb funcione)


#------------------------------???? no hacer caso de momento

#la señal control+\ no se ignora si hay un cat(comando interactivo)
