#include "minishell.h"

int	builtin(char **cmd, t_info *info, int *fd)
{
	int	cmd_len;

	cmd_len = ft_strlen(cmd[0]);
	if (!ft_strncmp(cmd[0], "cd", cmd_len))
		cd(cmd[1], info);
	else if (!ft_strncmp(cmd[0], "pwd", cmd_len))
		pwd(fd);
	else if (!ft_strncmp(cmd[0], "export", cmd_len))
		export(cmd, info);
	else if (!ft_strncmp(cmd[0], "unset", cmd_len))
		unset(cmd, info);
	else if (!ft_strncmp(cmd[0], "env", cmd_len))
		env(info, fd);
	else if (!ft_strncmp(cmd[0], "exit", cmd_len))
			execute_exit(cmd);
	else
	{
		free_double_string(cmd);
		return (FALSE);//명령어 없을 때
	}
	free_double_string(cmd);
	return (TRUE);
}
