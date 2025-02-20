NAME = pipex
NAME_BONUS = pipex_b
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3
RM = rm -f

SRC = pipex.c \

SRC_BONUS = pipex_bonus.c utils_bonus.c \

INCLUDE = -I libft -I.

LIB = libft/libft.a

OBJ = $(SRC:.c=.o)

OBJ_BONUS = $(SRC_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

$(NAME_BONUS): $(OBJ_BONUS)
	make -C libft
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIB) -o $(NAME_BONUS)

bonus: $(NAME_BONUS)

clean:
	$(RM) $(OBJ) $(OBJ_BONUS)
	make clean -C libft

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re bonus
