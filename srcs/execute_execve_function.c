#include "minishell.h"

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

void	execute_input_command(t_info *info, char **command, int fd)
{
	int		pid;
	int		exit_status_of_child;
	char	*path_of_commnad;

	pid = fork();
	if (pid == -1)
		print_error("fork error");
	else if (pid > 0)
	{
		if (waitpid(pid, &exit_status_of_child, 0) == -1)
			print_error("waitpid error");
	}
	else if (pid == 0)
	{
		if (info->n_pipeline)
		{
			dup2(fd, 0);
			close(fd);
		}
		path_of_commnad
			= get_path_of_command(info->env_path, command[0]);
		execve(path_of_commnad, command, info->env_list);
		free(path_of_commnad);
		print_error(command[0]);
	}
}

void	execute_output_command(t_info *info, char **command, int fd)
{
	int		pid;
	int		exit_status_of_child;
	char	*path_of_commnad;

	pid = fork();
	exit_status_of_child = 0;
	if (pid == -1)
		print_error("fork error");
	else if (pid > 0)
	{
		if (waitpid(pid, &exit_status_of_child, 0) == -1)
			print_error("waitpid error");
	}
	else if (pid == 0)
	{
		if (info->n_pipeline)
		{
			dup2(fd, 1);
			close(fd);
		}
		path_of_commnad
			= get_path_of_command(info->env_path, command[0]);
		execve(path_of_commnad, command, info->env_list);
		// builtin(info);//부모 -> 표준출력
		free(path_of_commnad);
		print_error(command[0]);
	}
}
