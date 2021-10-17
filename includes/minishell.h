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

#define KEY 0
#define VALUE 1

# define FRONT 0
# define MID 1
# define BACK 2

# define BUF_SIZE 20000
# define ISODD 1

# define MAX_FD 256

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
** Union type Definition
** =============================================================================
*/

typedef union u_exit
{
	int				num;
	unsigned char	c[4];
}			t_exit;

/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

typedef struct s_env
{
	char			*key;
	char			*value;
	int				env_flag;
	struct s_env	*next;
	struct s_env	*prev;
}			t_env;

typedef struct s_env_deq
{
	int		size;
	t_env	*head;
	t_env	*last;
}			t_env_deq;

typedef struct s_pipex
{
	int		**pipe_fd;
	int		*pid;
	char	*limiter;
	int		is_here_doc;
}	t_pipex;

typedef struct s_quote
{
	int	squote_flag;
	int	dquote_flag;
	int	squote_cnt;
	int	dquote_cnt;
}			t_quote;

typedef struct s_lst
{
	char			*str;
	struct s_lst	*next;
}			t_lst;

typedef struct s_cmd
{
	t_lst	*text;
	t_lst	*redi;
}			t_cmd;

typedef struct s_info
{
	char		**env_path;
	int			cmd_sequence;
	int			n_cmd;
	int			n_pipe;
	char		*infile_name;
	char		*outfile_name;
	char		*env_list;
	t_env_deq	*env_deq;
	t_cmd		*cmd_lst;
	t_pipex 	pipex;
}			t_info;

/*
** =============================================================================
** FILES
** =============================================================================
*/

//execute
void	execute_command_main(t_info *info);
void	execute_command(t_info *info, int depth);
void	execute_execve_function(t_info *info, int depth);
int		is_builtin_command(t_info *info);

void	get_pipe_fd(t_info *info, int depth, int fd[]);
void	switch_stdio(t_info *info, int fd_stdin, int fd_stdout);

void	make_pipeline(t_info *info);
void	close_pipeline(t_info *info);

//redirection
int		redirection(t_info *info, int fd[]);

//free
void	free_two_dimensional(char **two_dimensional);

//builtin
int		cd(char *path, t_info *info);
int		pwd(int *fd);
void	execute_exit(char **cmd);

//export
int		incorrect_env_key(char *env_key);
t_env	*check_listin(char *env_key, t_info *info);
void	export(char **cmd, t_info *info);

void	unset(char **cmd, t_info *info);

void	env(t_info *info, int *fd);

void	save_env_variables(t_info *info, char **envp);
void	make_env_double_string(t_info *info);

void	sig_handler(int signo);

int		builtin(char **cmd, t_info *info, int *fd);

void	free_double_string(char **list);

//parsing
int		is_spacial(char c);
int		parse_line(char *line, t_info *info);

//env_list, utils
char	**env_split(char *str);
char	*get_env_value(char *key, t_info *info);

t_env	*create_env_node(void);
void	link_env_node(t_env *front, t_env *back);
t_env	*make_env_list(char **envp);
int		is_register_variable(char *cmd);
void	register_variable(char *cmd, t_info *info);



//list.c
t_lst	*create_node(void);
void	link_node(char *cmd, t_lst **list);

//error
int		error(void);
void	merror(void *addr);
void	error_message(char *cmd, char *arg, char *msg);

#endif
