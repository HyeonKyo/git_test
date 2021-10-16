#include "minishell.h"

int	cd(char *path, t_info *info)
{
	char	*home;

	if (path == NULL)
	{
		home = get_env_value("HOME", info);
		chdir(home);
		free(home);
		return (NORMAL);
	}
	if (chdir(path) == -1)
		return (ERROR);
	return (NORMAL);
}

/*
1. (상대, 절대)경로 문자열을 인자로 받음.
2. cd성공시 0리턴, 실패시 1리턴
*/
