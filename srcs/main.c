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
	info->cmd = (char **)malloc(sizeof(char * ) * 4);
	merror(info->cmd);
	info->cmd[0] = line;
	info->cmd[1] = ft_strdup("ls"); //파이프라인 시험해볼려고 임시로 명령어 넣어놓음
	info->cmd[2] = ft_strdup("wc -l");//파이프라인 시험해볼려고 임시로 명령어 넣어놓음
	info->cmd[3] = NULL;
	// info->command = ft_split(line, ' ');//파싱 함수로 대체
	if (line)
		add_history(line);//히스토리 저장은 어디에 되는지?
	info->pipe_cnt = 0;
	info->cmd_cnt = 1;
	// info->cmd_sequence = 0;
	// int	fd[2] = {0, 1};
	// builtin(info, fd);
	execute_command_main(info, 0);
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
