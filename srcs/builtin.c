#include "minishell.h"

int	builtin(t_info *info, int *fd)
{
	char	**cmd;
	int		cmd_len;

	cmd = ft_split(info->cmd[info->cmd_sequence], ' ');
	cmd_len = ft_strlen(cmd[0]);
	if (!ft_strncmp(cmd[0], "cd", cmd_len))
		cd(cmd[1]);
	else if (!ft_strncmp(cmd[0], "pwd", cmd_len))
		pwd(fd);
	else if (!ft_strncmp(cmd[0], "export", cmd_len))
		export(cmd, info);
	else
	{
		free_env_list(cmd);
		return (FALSE);//명령어 없을 때
	}
	free_env_list(cmd);
	return (TRUE);
}
