#include "minishell.h"

char	*get_cmd_path(char **env_path, t_info *info)
{
	int			idx;
	char		*cmd;
	char		*path_of_cmd;
	struct stat	file_stat;

	idx = 0;
	path_of_cmd = NULL;
	cmd = info->cmd_lst[info->cmd_sequence].text->str;
	if (info->cmd_lst[info->cmd_sequence].text->str[0] == '\0')
		cmd = info->cmd_lst[info->cmd_sequence].text->next->str;
	while (env_path[idx])
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

	cmd = info->cmd_list[0];
	cmd_len = ft_strlen(cmd);
	if (cmd_len <= 0)
		return (FALSE);
	if (!ft_strncmp(cmd, "cd", cmd_len)
		|| !ft_strncmp(cmd, "pwd", cmd_len)
		|| !ft_strncmp(cmd, "export", cmd_len)
		|| !ft_strncmp(cmd, "unset", cmd_len)
		|| !ft_strncmp(cmd, "env", cmd_len)
		|| !ft_strncmp(cmd, "exit", cmd_len)
		|| is_register_variable(cmd))
		return (TRUE);
	return (FALSE);
}

int	execute_execve(t_info *info, int depth)
{
	int		fd[2];
	char	*cmd_path;

	get_pipe_fd(info, depth, fd);
	if (redirection(info, fd))
		return (ERROR);//비정상 종료 리턴
	cmd_path = get_cmd_path(info->env_path, info);
	if (get_cmd_list(info) == -1)
		return (ERROR);
	if (!is_builtin_command(info) || !(info->n_cmd == 1))
		switch_stdio(info, fd[READ], fd[WRITE]);
	if (is_builtin_command(info))//**현교 : 이 if문 한 블록을 builtin함수 안에 넣어도 될듯?
	{
		builtin(info->cmd_list, info, fd);
		if (info->n_cmd > 1)
			exit(EXIT_SUCCESS);
		/*execve()는 알아서 프로세스가 교체되지만 builtin함수는 직접 exit을 해줘야한다. 안그러면 무한반복
		커맨드가 하나 일 때는 부모에서 실행되기 때문에 exit되면 안됨*/
	}
	else
	{
		execve(cmd_path, info->cmd_list, info->env_list);
		error_message(info->cmd_list[0], NULL,"command not found");
		exit(CMD_NOT_FOUND);//127 나중에 디파인상수로
		//비정상 종료 리턴
	}
	return (NORMAL);
}
