# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "../Libft/libft.h"

typedef struct s_minishell
{
	char	**environment_path;
	char	*limiter;
	int		is_here_doc;
}	t_minishell;

void	free_two_dimensional(char **two_dimensional)
{
	int	idx;

	idx = 0;
	while (two_dimensional[idx])
	{
		if (!(two_dimensional[idx]))
			free(two_dimensional[idx]);
		idx++;
	}
	if (!(two_dimensional))
		free(two_dimensional);
}

static char	*get_path_of_command(char **environment_path, char *command)
{
	int		idx;
	int		end;
	char	*path_of_commnad;

	idx = 0;
	end = sizeof(environment_path);
	path_of_commnad = NULL;
	while (idx < end)
	{	
		if (access(command, F_OK) == 0)
			return (command);
		path_of_commnad = ft_strjoin(environment_path[idx], command);
		if (access(path_of_commnad, F_OK) == 0)
			return (path_of_commnad);
		free(path_of_commnad);
		idx++;
	}
	return (path_of_commnad);
}

static void	add_slash_at_end_of_path(t_minishell *minishell, char **environment_path)
{
	int		idx;
	char	*temp_for_substr;

	idx = 0;
	while (environment_path[idx])
	{	
		minishell->environment_path[idx] = ft_strjoin(environment_path[idx], "/");
		idx++;
	}
}

void	set_environment_path(t_minishell *minishell)
{
	int		idx;
	char	**environment_path;

	idx = 0;
	environment_path = ft_split(getenv("PATH"), ':');
	minishell->environment_path = (char **)malloc(sizeof(char *)
		* (sizeof(environment_path) + 1));
	add_slash_at_end_of_path(minishell, environment_path);
	minishell->environment_path[sizeof(environment_path)] = NULL;
	free_two_dimensional(environment_path);
}

void	get_line(t_minishell *minishell)
{
	char	*line;	
	char	**command;
	char	*path_of_commnad;
	char	current_path[1024];
	int		pid;
	int		exit_status_of_child;

	line = NULL;
	if (line)
	{
		free(line);
		line = NULL;
	}
	getcwd(current_path, 1024);
	if (!current_path)
		exit(1);
	line = readline("Minishell>");
	rl_on_new_line();
	rl_replace_line(current_path, 0);
	rl_redisplay();
	command = ft_split(line, ' ');
	if (line)
		add_history(line);
	pid = fork();
	if (pid > 0)
	{
		if (waitpid(pid, &exit_status_of_child, 0) == -1)
			exit(1);
	}
	else if (pid == 0)
	{
		path_of_commnad = get_path_of_command(minishell->environment_path, command[0]);
		execve(path_of_commnad, command, NULL);
		exit(1);
	}
}

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(void)
{
	char		*line;
	t_minishell	minishell;

	set_environment_path(&minishell);
	signal(SIGINT, sig_handler);
	while (1)
	{
		get_line(&minishell);
	}
}