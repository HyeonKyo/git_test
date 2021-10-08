NAME = minishell

SRCS = ./srcs/main.c
OBJS = $(SRCS:.c=.o)

INC	= ./includes/
LIBFT = ./Libft/
CC = gcc
CFLAGS = -g3 -fsanitize=address -lreadline -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib
#CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address -lncurses

all : $(NAME)

$(NAME) : $(SRCS)
	make all -C $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) ./Libft/libft.a -I $(INC)

fclean : clean
	make clean -C $(LIBFT)
	rm -rf $(NAME)

clean :
	rm -rf $(OBJS)

re :
	make fclean
	make all

.PHONY: clean fclean re all bonus
