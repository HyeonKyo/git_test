#include "minishell.h"

static char	*get_path_of_command(char **env_path, char *cmd)
{
	int		idx;
	int		end;
	char	*path_of_cmd;

	idx = 0;
	end = sizeof(env_path);//1.이러면 환경변수 개수 만큼 출력이 되는지?
	path_of_cmd = NULL;
	while (idx < end)
	{
		if (access(cmd, F_OK) == 0)//2.access함수 사용 가능한지?
			return (cmd);
		path_of_cmd = ft_strjoin(env_path[idx], cmd);
		if (access(path_of_cmd, F_OK) == 0)
			return (path_of_cmd);
		free(path_of_cmd);//3.path에 커맨드가 없는 경우 여기서 free된 path가 리턴되어 double free발생
		path_of_cmd = NULL;//때문에 NULL을 넣어주고 이후의 free 전에 널체크를 해서 방지함.
		idx++;
	}
	return (path_of_cmd);
}

void	execute_input_command(t_info *info, char **cmd, int fd_stdin, int fd_stdout)
{
	int		pid;
	int		exit_status_of_child;
	char	*path_of_cmd;
	int		fd[2];

	pid = fork();
	fd[0] = fd_stdin;
	fd[1] = fd_stdout;
	if (pid == -1)
		error();
	else if (pid > 0)
	{
		if (waitpid(pid, &exit_status_of_child, 0) == -1)
			error();
	}
	else if (pid == 0)
	{
		path_of_cmd
			= get_path_of_command(info->env_path, cmd[0]);
		switch_stdio(info, fd_stdin, fd_stdout);
		// execve(path_of_cmd, cmd, info->env_list);
		builtin(info, fd);
	}
}

void	execute_output_command(t_info *info, char **cmd, int fd_stdin, int fd_stdout)
{
	int		pid;
	int		exit_status_of_child;
	char	*path_of_cmd;
	int		fd[2];

	pid = fork();
	fd[0] = fd_stdin;
	fd[1] = fd_stdout;
	exit_status_of_child = 0;
	if (pid == -1)
		error();
	else if (pid > 0)
	{
		if (waitpid(pid, &exit_status_of_child, 0) == -1)
			error();
	}
	else if (pid == 0)
	{
		path_of_cmd
			= get_path_of_command(info->env_path, cmd[0]);
		switch_stdio(info, fd_stdin, fd_stdout);
		//execve(path_of_cmd, cmd, info->env_list);
		builtin(info, fd);
	}
}
