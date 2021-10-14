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
# include <sys/stat.h>
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

# define ISODD 1

/*
** =============================================================================
** Enum type Definition
** =============================================================================
*/

typedef enum e_type
{
	NORM = 0,
	SQUOTE, DQUOTE,
	SPCE, SPEC, DOLR, PIPE, END,
	RRDI, LRDI, DRRDI, DLRDI

}			t_type;


/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

typedef struct s_pipex
{
	int		**pipe_fd;
	int		*pid;
	char	*limiter;
	int		is_here_doc;
}	t_pipex;

typedef struct s_cmd
{
	char	**cmd;
	t_type	redi;
}			t_cmd;

typedef struct s_info
{
	char	**env_path;
	char	**env_list;
	t_cmd	*cmds;
	int		cmd_sequence;
	int		n_cmd;
	int		n_pipe;
	char	*infile_name;
	char	*outfile_name;
	t_pipex pipex;
}			t_info;

/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

void	execute_command_main(t_info *info);
void	execute_command(t_info *info, int depth);
void	execute_execve_function(t_info *info, int depth);
int		get_fd_will_be_stdin(t_info *info, int depth, int is_redirection);
int		get_fd_will_be_stdout(t_info *info, int depth, int is_redirection);
void	switch_stdio(t_info *info, int fd_stdin, int fd_stdout);

void	make_pipeline(t_info *info);
void	close_pipeline(t_info *info);

int		is_builtin_command(t_info *info);

void	free_two_dimensional(char **two_dimensional);

char	*make_cursor_string(void);

//builtin
int		cd(char *path);
int		pwd(int *fd);

int		check_listin(char *env_key, t_info *info);
char	*get_env_value(char *env_key, t_info *info);
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

//parsing
int		parse_line(char *line, t_info *info);

#endif
