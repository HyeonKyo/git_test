#include "minishell.h"

char	*get_path_of_command(char **env_path, char *cmd)
{
	int		idx;
	int		end;
	char	*path_of_cmd;
	struct stat file_stat;

	idx = 0;
	end = sizeof(env_path);//1.이러면 환경변수 개수 만큼 출력이 되는지?
	path_of_cmd = NULL;
	while (idx < end)
	{
		if (stat(cmd, &file_stat) == 0)
			return (cmd);
		path_of_cmd = ft_strjoin(env_path[idx], cmd);
		if (stat(path_of_cmd, &file_stat) == 0)
			return (path_of_cmd);
		free(path_of_cmd);//3.path에 커맨드가 없는 경우 여기서 free된 path가 리턴되어 double free발생
		path_of_cmd = NULL;//때문에 NULL을 넣어주고 이후의 free 전에 널체크를 해서 방지함.
		idx++;
	}
	return (path_of_cmd);
}

int	is_builtin_command(t_info *info)
{
	char	*cmd;
	int		cmd_len;

	cmd = info->cmd_lst[info->cmd_sequence].text->str;
	cmd_len = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "cd", cmd_len)
		|| !ft_strncmp(cmd, "pwd", cmd_len)
		|| !ft_strncmp(cmd, "export", cmd_len)
		|| !ft_strncmp(cmd, "unset", cmd_len)
		|| !ft_strncmp(cmd, "env", cmd_len)
		|| !ft_strncmp(cmd, "exit", cmd_len))
		return (TRUE);
	return (FALSE);
}

void	malloc_cmd_list(t_info *info, char ***cmd_list)
{
	int		cnt;
	t_lst	*cur;
	t_lst	*next;

	cnt = 0;
	cur = info->cmd_lst[info->cmd_sequence].text;
	while (cur != NULL)
	{
		next = cur->next;
		cur = next;
		cnt++;
	}
	*cmd_list = (char **)malloc(sizeof(char *) * cnt + 1);
}

char	**set_cmd_list(t_info *info)
{
	int		cnt;
	t_lst	*cur;
	t_lst	*next;
	char	**cmd_list;

	cnt = 0;
	malloc_cmd_list(info, &cmd_list);
	merror(cmd_list);
	cur = info->cmd_lst[info->cmd_sequence].text;
	while (cur != NULL)
	{
		if (*cur->str == 0)
			break;
		cmd_list[cnt] = cur->str;
		next = cur->next;
		cur = next;
		cnt++;
	}
	cmd_list[cnt] = NULL;
	return (cmd_list);
}

void	execute_execve_function(t_info *info, int depth)
{
	char	*path_of_cmd;
	char	**cmd_list;
	int		fd[2];

	fd[READ] = get_fd_will_be_stdin(info, depth, 0);
	fd[WRITE] = get_fd_will_be_stdout(info, depth, 0);
	path_of_cmd
		= get_path_of_command(info->env_path, info->cmd_lst[depth].text->str);
	cmd_list = set_cmd_list(info);
	switch_stdio(info, fd[READ], fd[WRITE]);
	if (is_builtin_command(info))
	{
		// builtin(info, fd);
		// if (info->n_cmd > 1)
			// exit(EXIT_SUCCESS);
		/*execve()는 알아서 프로세스가 교체되지만 builtin함수는 직접 exit을 해줘야한다. 안그러면 무한반복
		커맨드가 하나 일 때는 부모에서 실행되기 때문에 exit되면 안됨*/

	}
	// else if (is_redirection_command(info))
	// { 빌트인이나 리다이렉션이 아니면 자식 프로세스 만들어서 작업
	// 	execute_redirection(info);
	// }
	else
	{
		execve(path_of_cmd, cmd_list, info->env_list);
		error();
	}
}
