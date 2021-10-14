CC			=	gcc
CFLAGS		=	-g -fsanitize=address -lreadline -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include
RM			=	rm -rf

NAME		=	minishell

INC			=	./includes/

LIBFT		=	./Libft/

SRCS_DIR	=	./srcs/
OBJS_DIR	=	./objs/

MAIN_DIR	=	main/
BLTIN_DIR	=	builtin/
ENV_DIR		=	env/
EXEC_DIR	=	execute/
FREE_DIR	=	free/
PARSE_DIR	=	parse/
SIG_DIR		=	signal/
UTL_DIR		=	utility/



OBJS_LST	=	main.o						\
				builtin.o					\
				cd.o						\
				env.o						\
				export.o					\
				free.o						\
				pwd.o						\
				unset.o						\
				save_env_values.o			\
				close_pipeline.o			\
				execute_execve_function.o	\
				execute_shell_command.o		\
				get_fd_will_be_stdio.o		\
				make_pipeline.o				\
				switch_stdio.o				\
				free_two_dimensional.o		\
				parse.o						\
				signal.o					\
				error.o

OBJS		=	$(addprefix $(OBJS_DIR), $(OBJS_LST))


$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(MAIN_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(BLTIN_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(ENV_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(EXEC_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(FREE_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(PARSE_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(SIG_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(UTL_DIR)%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -o $@ -c $< $(LIBFT)libft.a -I$(INC)


# CFLAGS = -g3 -fsanitize=address -lreadline -L./readline/lib
#CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address -lncurses

all				:	$(NAME)

$(NAME)			:	$(OBJS)
					make all -C $(LIBFT)
					$(CC) $(CFLAGS) -o $@ $^ $(LIBFT)libft.a -I$(INC)

fclean			:	clean
					make fclean -C $(LIBFT)
					$(RM) $(NAME)

clean			:
					$(RM) $(OBJS_DIR)

re				:
					make fclean
					make all

.PHONY			:	clean fclean re all bonus
