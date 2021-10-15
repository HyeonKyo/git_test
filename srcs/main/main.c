# include "minishell.h"

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
	int		pid;
	char	*str;

	line = NULL;
	if (line)
	{
		free(line);
		line = NULL;
	}
	//함수로 따로 따기
	// str = make_cursor_string();//주소 넣을거면 이걸로
	str = "Minishell >";
	line = readline(str);
	if (line == NULL)//EOF(ctrl + d)만나면 NULL
	{
		ft_putstr_fd("\x1b[1A", STDOUT_FILENO);
		ft_putstr_fd("\033[12C", STDOUT_FILENO);
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	if (*line == 0)//엔터 눌렀을 때 함수 종료
	{
		free(line);
		return ;
	}
	if (line)
		add_history(line);
	if (parse_line(line, info))
		error();
	execute_command_main(info);
	return ;
}

int	main(int arc, char *arvg[], char *envp[])
{
	char	*line;
	t_info	info;

	info.pipex.is_here_doc = 0;
	save_env_values(&info, envp);
	set_environment_path(&info);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while (TRUE)
	{
		get_line(&info);
	}
}
