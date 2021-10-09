NAME = minishell

SRCS = 	./srcs/main.c\
		./srcs/execute_shell_command.c ./srcs/command_of_pipeline.c\
		./srcs/execute_execve_function.c ./srcs/get_fd_will_be_stdio.c\
		./srcs/free_two_dimensional.c				\
		./srcs/print_error.c		\
		./srcs/cd.c			\
		./srcs/pwd.c			\
		./srcs/signal.c			\
		./srcs/error.c			\
		./srcs/save_env_values.c
OBJS = $(SRCS:.c=.o)

INC	= ./includes/
LIBFT = ./Libft/
CC = gcc
CFLAGS = -g3 -fsanitize=address -lreadline -L./readline/lib
#CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address -lncurses

all : $(NAME)

$(NAME) : $(SRCS)
	make all -C $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) ./Libft/libft.a -I$(INC)

fclean : clean
	make clean -C $(LIBFT)
	rm -rf $(NAME)

clean :
	rm -rf $(OBJS)

re :
	make fclean
	make all

.PHONY: clean fclean re all bonus
