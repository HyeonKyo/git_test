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
	char	**cmd;
	int		cmd_len;

	cmd = ft_split(info->cmd[info->cmd_sequence], ' ');
	cmd_len = ft_strlen(cmd[0]);
	if (!ft_strncmp(cmd[0], "cd", cmd_len))
		return (TRUE);
	else if (!ft_strncmp(cmd[0], "pwd", cmd_len))
		return (TRUE);
	else if (!ft_strncmp(cmd[0], "export", cmd_len))
		return (TRUE);
	else if (!ft_strncmp(cmd[0], "unset", cmd_len))
		return (TRUE);
	else if (!ft_strncmp(cmd[0], "env", cmd_len))
		return (TRUE);
	else if (!ft_strncmp(cmd[0], "exit", cmd_len))
		return (TRUE);
	return (FALSE);
}

void	execute_execve_function(t_info *info, int depth)
{
	char	*path_of_commnad;
	char	**command;
	int		fd[2];

	fd[READ] = get_fd_will_be_stdin(info, depth, 0);
	fd[WRITE] = get_fd_will_be_stdout(info, depth, 0);
	command = ft_split(info->cmd[depth], ' ');
	path_of_commnad
		= get_path_of_command(info->env_path, command[0]);
	switch_stdio(info, fd[READ], fd[WRITE]);
	// if (is_redirection_command(info))
	// { 빌트인이나 리다이렉션이 아니면 자식 프로세스 만들어서 작업
	// 	execute_redirection(info);
	// }
	if (is_builtin_command(info))
		builtin(info, fd);
	else
		execve(path_of_commnad, command, info->env_list);
}