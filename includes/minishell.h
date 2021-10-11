#ifndef info_H
# define info_H

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

typedef struct s_info
{
	char	**env_path;
	char	**env_list;
	char	**cmd;
	int		cmd_sequence;
	int		cmd_total_number;
	int		n_pipeline;
	char	*infile_name;
	char	*outfile_name;
	t_pipex pipex;
}			t_info;

/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

int		execute_shell_command(t_info *info, int depth);
void	execute_command(t_info *info, char **cmd, int fd[]);
// void	execute_input_command(t_info *info, char **cmd, int fd[]);
// void	execute_output_command(t_info *info, char **cmd, int fd[]);
int		get_fd_will_be_stdin(t_info *info, int is_redirection);
int		get_fd_will_be_stdout(t_info *info, int is_redirection);
void	print_error(char *output_string);
void	free_two_dimensional(char **two_dimensional);
void	switch_stdio(t_info *info, int fd_stdin, int fd_stdout);

char	*make_cursor_string(void);

//builtin
int		cd(char *path);
int		pwd(int *fd);

int		check_listin(char *env_value, t_info *info);
int		get_env_list_size(char **env_list);
void	export(char **cmd, t_info *info);

void	unset(char **cmd, t_info *info);

void	env(t_info *info, int *fd);

int		error(void);
void	merror(void *addr);

void	save_env_values(t_info *info, char **envp);

void	sig_handler(int signo);

int		builtin(t_info *info, int *fd);

void	free_env_list(char **list);

#endif
