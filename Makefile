NAME = minishell

SRCS = ./srcs/main.c
OBJS = $(SRCS:.c=.o)

INC	= ./inc/
LIBFT = ./Libft/
CC = gcc
CFLAGS = -g3 -fsanitize=address -lncurses -lreadline
#CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address -lncurses

all : $(NAME)

$(NAME) : $(SRCS)
	make all -C $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) ./Libft/libft.a -I $(INC)

fclean : clean
	rm -rf $(NAME)

clean : 
	rm -rf $(OBJS)

re :
	make fclean
	make all

.PHONY: clean fclean re all bonus