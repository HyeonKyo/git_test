#include "minishell.h"

int	pwd(int *fd)
{
	char	*cur_path;

	cur_path = getcwd(NULL, 0);
	if (cur_path == NULL)
		return (error());
	ft_putendl_fd(cur_path, fd[WRITE]);
	free(cur_path);
	return (NORMAL);
}

void	builtin(t_info *info, int *fd)
{
	char	**cmd;
	int		cmd_len;

	cmd = ft_split(info->cmd[info->cmd_sequence], ' ');
	cmd_len = ft_strlen(cmd[0]);
	if (ft_strncmp(cmd[0], "cd", cmd_len))
		cd(cmd[1]);
	else if (ft_strncmp(cmd[0], "pwd", cmd_len))
		pwd(fd);
}