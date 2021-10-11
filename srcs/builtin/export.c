#include "minishell.h"

int	check_listin(char *env_value, t_info *info)
{
	int	i;
	int	len_value;

	len_value = (int)ft_strlen(env_value);
	while (info->env_list[i] != NULL)
	{
		if (!ft_strncmp(env_value, info->env_list[i], len_value))
		{
			if (info->env_list[i][len_value] == '=')
				return (i);
		}
		i++;
	}
	return (-1);
}

int	get_env_list_size(char **env_list)
{
	int	list_len;

	list_len = 0;
	while (env_list[list_len++])
		;
	return (list_len);
}

int	make_new_list(t_info *info)
{
	int		i;
	int		list_len;
	char	**env_list;
	char	**new;

	env_list = info->env_list;
	list_len = get_env_list_size(env_list);
	new = (char **)malloc(sizeof(char *) * (list_len + 1));
	merror(new);
	i = -1;
	while (++i < list_len)
		new[i] = env_list[i];
	new[i] = NULL;
	free_env_list(env_list);
	info->env_list = new;
	return (list_len);
}

void	add_env_value(int idx, char **env, t_info *info)
{
	char	*env_str;

	if (env[1] == NULL)
		return ;
	env_str = ft_strjoin(env[0], env[1]);
	merror(env_str);
	free(info->env_list[idx]);
	info->env_list[idx] = env_str;
}

void	add_new_env(char **cmd, t_info *info)
{
	int	list_len;

	list_len = make_new_list(info);
	info->env_list[--list_len] = ft_strdup((char *)cmd[1]);
	merror(info->env_list[--list_len]);
}

void	export(char **cmd, t_info *info)
{
	int		idx;
	char	**env;

	env = ft_split(cmd[1], '=');
	merror(env);
	idx = check_listin(env[0], info);
	if (idx >= 0)
		add_env_value(idx, env, info);
	else
		add_new_env(cmd, info);
}

/*
cmd[0] = export
cmd[1] = QQ or QQ=asd
1. =앞의 변수 이름 QQ가 이미 환경변수에 있는지 확인
	환경변수list마다 변수 길이만큼 strncmp
		-> 통과하면 변수길이 다음 문자가 =인지 확인
			-> 있으면 그 값만 추가하거나 변경
2. 없으면 그냥 추가
	1. 환경변수 리스트 확장
	2. QQ=~~그대로 추가.

*/
