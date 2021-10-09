#ifndef MINISHELL_H
# define MINISHELL_H

/*
** =============================================================================
** Dependencies
** =============================================================================
*/

# include <stdio.h>
# include "readline/history.h"
# include "readline/readline.h"
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"

/*
** =============================================================================
** Macro Declaration
** =============================================================================
*/

# define TRUE 1
# define FALSE 0

# define ERROR 1
# define NORMAL 0

# define READ 0
# define WRITE 1

/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

typedef struct s_pipex
{
	int		new_pipe[2];
	int		old_pipe[2];
	char	*limiter;
	int		is_here_doc;
}	t_pipex;

typedef struct s_minishell
{
	char	**environment_path;
	char	**environment;
	char	**command;
	int		command_sequence;
	int		command_total_number;
	int		number_of_pipeline;
	char	*infile_name;
	char	*outfile_name;
	t_pipex pipex;
}			t_minishell;

/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

void	execute_shell_command(t_minishell *minishell, int depth);
void	input_command_of_pipeline(t_minishell *minishell, int is_redirection);
void	output_command_of_pipeline(t_minishell *minishell, int is_redirection);
void	execute_input_command(t_minishell *minishell, char **command, int fd);
void	execute_output_command(t_minishell *minishell, char **command, int fd);
void	print_error(char *output_string);
int		get_fd_will_be_stdin(t_minishell *minishell, int is_redirection);
int		get_fd_will_be_stdout(t_minishell *minishell, int is_redirection);
void	free_two_dimensional(char **two_dimensional);

#endif
