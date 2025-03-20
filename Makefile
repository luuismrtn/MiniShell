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
	src/builtins/modify_pwd.c \
	src/builtins/modify_pwd_utils.c \

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

# MASS PRUEBAS CON ERROR:

#heredoc control c tiene que salir

#cd ../aldferna/.. --> no such file or directory:
#bash:
#aldferna@c2r7s6:~$ cd ../aldferna/..
#aldferna@c2r7s6:/home$ 

#pwd | cat | cat -e >3 | wc -l (bash: 0 y crea 3 con print dwl pwd) (mshell: hace el pwd y no crea 3)

#actualizar el echo $?

#<<eof cat | ls (bash pilla eof como delimitador y al acabar heredoc hace ls) 
#(mshell pilla: eofcat como delimit y al cerrar se queda en el aire)
#<<eof cat | cat -e | cat -e (igual)
#<<eof cat | cat (igual)

#en lo de crear dir, meterse y luego salir, al encontrar un path que existe pone cosas raras en el readline

#exit 3 | ls  (no hace el ls)

#/home/aldferna/Desktop/MiniShell ~ cat <2
#minishell: open: No such file or directory  (ok error, pero se queda pillado)




#cosaas q funcionaban el martes:

#cat | cat | ls  (no cierra)

#esto es bash:: da el codigo del ultimo comando escrito
# aldferna@c2r7s6:~/Desktop/MiniShell$ exit 5 | ls
# inc  libft  Makefile  minishell  src
# aldferna@c2r7s6:~/Desktop/MiniShell$ echo $?            ----nos da 1
# 0

# aldferna@c2r7s6:~/Desktop/MiniShell$ sdffggb | sleep 1
# sdffggb: command not found
# aldferna@c2r7s6:~/Desktop/MiniShell$ echo $?              ----nos da 1
# 0

# aldferna@c2r7s6:~/Desktop/MiniShell$ sleep 1  | sdfg
# Command 'sdfg' not found, did you mean:
#   command 'asdfg' from deb aoeui (1.7+20160302.git4e5dee9-3)
#   command 'sdf' from deb sdf (2.001+1-8)
#   command 'sdig' from deb pdns-tools (4.5.3-1)
# Try: apt install <deb name>
# aldferna@c2r7s6:~/Desktop/MiniShell$ echo $?             ----nos da 0
# 127

#exit 5 |exit 4 | exit 3 --> echo $? debe dar 3





#--------------------cosas de las q podriamos pasar:

#echo $"$'$PWD'"  (bash: $'/home/aldferna/Desktop/MiniShell') (mshell: $$'/home/aldferna/Desktop/MiniShell')

# export q="ls -la"
# $q  (esto bash lo hace, pero me ha dicho que el 50% de las mini no lo hacen)
