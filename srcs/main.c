# include "minishell.h"

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

static void	add_slash_at_end_of_path(t_info *info, char **environment_path)
{
	int		idx;
	char	*temp_for_substr;

	idx = 0;
	while (environment_path[idx])
	{
		info->env_path[idx] = ft_strjoin(environment_path[idx], "/");
		idx++;
	}
}

void	set_environment_path(t_info *info)
{
	int		idx;
	char	**environment_path;

	idx = 0;
	environment_path = ft_split(getenv("PATH"), ':');
	info->env_path = (char **)malloc(sizeof(char *)
		* (sizeof(environment_path) + 1));
	add_slash_at_end_of_path(info, environment_path);
	info->env_path[sizeof(environment_path)] = NULL;
	free_two_dimensional(environment_path);
}

void	get_line(t_info *info)
{
	char	*line;
	char	**command;
	char	*current_path;
	int		pid;
	char	*str;

	line = NULL;
	if (line)
	{
		free(line);
		line = NULL;
	}
	//함수로 따로 따기
	current_path = getcwd(NULL, 0);//상대경로(마지막 슬래쉬)파싱 함수 만들기
	str = ft_strjoin("info ", current_path);
	str = ft_strjoin(str, ">");
	line = readline(str);
	if (line == NULL)//EOF(ctrl + d)만나면 NULL
		exit(0);
	info->cmd = (char **)malloc(sizeof(char * ) * 2);
	merror(info->cmd);
	*info->cmd = line;
	*(info->cmd + 1) = NULL;
	// info->command = ft_split(line, ' ');//파싱 함수로 대체
	if (line)
		add_history(line);//히스토리 저장은 어디에 되는지?
	info->n_pipeline = 0;
	execute_shell_command(info, info->n_pipeline);
}

int	main(int arc, char *arvg[], char *envp[])
{
	char	*line;
	t_info	info;

	save_env_values(&info, envp);
	set_environment_path(&info);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while (1)
	{
		get_line(&info);
	}
}
