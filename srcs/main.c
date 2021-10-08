# include "minishell.h"

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
	char	*path_of_command;
	char	*current_path;
	int		pid;
	int		exit_status_of_child;
	char	*str;

	line = NULL;
	if (line)
	{
		free(line);
		line = NULL;
	}
	//함수로 따로 따기
	current_path = getcwd(NULL, 0);//상대경로(마지막 슬래쉬)파싱 함수 만들기
	str = ft_strjoin("Minishell ", current_path);
	str = ft_strjoin(str, ">");
	line = readline(str);
	command = ft_split(line, ' ');//파싱 함수로 대체
	if (line)
		add_history(line);//히스토리 저장은 어디에 되는지?
	pid = fork();
	if (pid > 0)//자식 프로세스를 기다리는 곳, waitpid함수 실패 시 종료 -> 생각해보기
	{
		if (waitpid(pid, &exit_status_of_child, 0) == -1)
			exit(1);
	}
	else if (pid == 0)
	{
		path_of_command = get_path_of_command(minishell->environment_path, command[0]);
		execve(path_of_command, command, NULL);
		//빌트인 명령 실행 부분으로 넘어가기
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
