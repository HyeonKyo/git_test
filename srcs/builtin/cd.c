#include "minishell.h"

static int	cd_old_pwd(t_info *info)
{
	char	*old_pwd;

	old_pwd = NULL;
	old_pwd = get_env_value("OLDPWD", info);
	if (old_pwd == NULL)
	{
		error_message("cd", NULL, "OLDPWD not set");
		return (ERROR);
	}
	if (chdir(old_pwd) == ERROR)
	{
		error_message("cd", old_pwd, "No such file or directory");
		return (ERROR);
	}
	return (TRUE);
}

static void	save_old_pwd(t_info *info)
{
	char	*cmd[3];
	char	*tmp;

	tmp = NULL;
	cmd[0] = "export";
	tmp = getcwd(NULL, 0);
	if (tmp)
	{
		cmd[1] = ft_strjoin("OLDPWD=", tmp);
		free(tmp);
	}
	cmd[2] = NULL;
	export(cmd, info);
	free(cmd[1]);
}

void	cd(char *path, t_info *info)
{
	int			normal_flag;
	char		*home;

	normal_flag = FALSE;
	home = NULL;
	if (path == NULL || (path[0] == '~' && path[1] == 0))
	{
		home = get_env_value("HOME", info);
		if (home == NULL)
			return (error_message("cd", NULL, "Home not set"));
		if (chdir(home) == ERROR)
			error_message("cd", home, "No such file or directory");
		else
			normal_flag = TRUE;
		free(home);
	}
	else if (path[0] == '-' && path[1] == 0)
	{
		if (cd_old_pwd(info) != ERROR)
			normal_flag = TRUE;
	}
	else if (chdir(path) == ERROR)
		return (error_message("cd", path, "No such file or directory"));
	if (normal_flag)
		save_old_pwd(info);
}

/*
1. (상대, 절대)경로 문자열을 인자로 받음.
2. cd성공시 0리턴, 실패시 1리턴
*/
