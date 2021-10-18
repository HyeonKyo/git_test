# =============================================================================
# Color Variables
# =============================================================================

BLACK		= 	"\033[0;30m"
GRAY		= 	"\033[1;30m"
RED			=	"\033[1;31m"
GREEN		=	"\033[1;32m"
YELLOW		=	"\033[1;33m"
PURPLE		=	"\033[0;35m"
CYAN		=	"\033[1;36m"
WHITE		=	"\033[1;37m"
EOC			=	"\033[0;0m"
LINE_CLEAR	=	"\x1b[1A\x1b[M"

# =============================================================================
# Command Variables -fsanitize=address
# =============================================================================

CC			=	gcc
CFLAGS		=	-g -fsanitize=address -lreadline -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include
RM			=	rm -rf

# =============================================================================
# File Variables
# =============================================================================

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
				exit.o						\
				export.o					\
				free.o						\
				pwd.o						\
				unset.o						\
				save_env_values.o			\
				close_pipeline.o			\
				execute_execve_function.o	\
				execute_shell_command.o		\
				get_pipe_fd.o				\
				make_pipeline.o				\
				switch_stdio.o				\
				redirection.o				\
				free_two_dimensional.o		\
				parse.o						\
				signal.o					\
				error.o						\
				env_list.o					\
				env_utils.o					\
				get_next_line.o				\
				get_next_line_utils.o		\
				list.o
OBJS		=	$(addprefix $(OBJS_DIR), $(OBJS_LST))

# =============================================================================
# Target Generating
# =============================================================================

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(MAIN_DIR)%.c
					@make all -C $(LIBFT)
					@mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(BLTIN_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(ENV_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(EXEC_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(FREE_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(PARSE_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(SIG_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(OBJS_DIR)%.o	:	$(SRCS_DIR)$(UTL_DIR)%.c
					# mkdir -p $(OBJS_DIR)
					@$(CC) $(CFLAGS) -I$(INC) -c $(LIBFT)libft.a $< -o $@
					@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)

$(NAME)			:	$(OBJS)
					@echo $(GREEN) "Source files are compiled!\n" $(EOC)
					@echo $(WHITE) "Building '$(NAME)' for" $(YELLOW) "Mandatory" $(WHITE) "..." $(EOC)
					@$(CC) $(CFLAGS) -o $@ $^ $(LIBFT)libft.a -I$(INC)
					@echo $(CYAN) "'$(NAME)'(execute file) is created!\n" $(EOC)

# =============================================================================
# Rules
# =============================================================================

all				:	$(NAME)

clean			:
					@$(RM) $(OBJS_DIR)
					@echo $(RED) "Object files are cleaned!\n" $(EOC)

fclean			:	clean
					@make fclean -C $(LIBFT)
					@$(RM) $(NAME)
					@echo $(RED) "Execute file is removed!\n" $(EOC)

re				:
					make fclean
					make all

.PHONY			:	clean fclean re all bonus
