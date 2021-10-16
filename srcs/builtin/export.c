#include "minishell.h"

int	check_listin(char *env_key, t_info *info)
{
	int	i;
	int	len_value;

	len_value = (int)ft_strlen(env_key);
	i = 0;
	while (info->env_list[i] != NULL)
	{
		if (!ft_strncmp(env_key, info->env_list[i], len_value))
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
	free_double_string(env_list);
	info->env_list = new;
	return (list_len);
}

char	*get_env_value(char *env_key, t_info *info)
{
	int		env_idx;
	char	**env;
	char	*env_value;

	env_idx = check_listin(env_key, info);
	if (env_idx < 0)
		return (NULL);
	env = ft_split(info->env_list[env_idx], '=');
	merror(env);
	env_value = ft_strdup(env[1]);
	free_double_string(env);
	return (env_value);
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

void	add_new_env(char **cmd, t_info *info, int idx)
{
	int	list_len;

	list_len = make_new_list(info);
	info->env_list[--list_len] = ft_strdup((char *)cmd[idx]);
	merror(info->env_list[--list_len]);
}

int	incorrect_env_key(char *env_key)
{
	int	i;

	i = 0;
	if (ft_isdigit(env_key[i]))
		return (TRUE);
	while (env_key[i] && !is_spacial(env_key[i]))
		i++;
	if (env_key[i] == 0)
		return (FALSE);
	return (TRUE);
	//특수문자 말고 확인할거 있는지 체크하기
	// + 첫글자가 숫자인지도 체크
}

int	check_add_value(char **env)
{
	int	len;

	len = (int)ft_strlen(env[0]);
	//if (env[0][len - 1] == '+')
}

void	export(char **cmd, t_info *info)
{
	int		i;
	int		env_idx;
	int		add_flag;
	char	**env;

	i = 0;
	add_flag = FALSE;
	while (cmd[++i] != NULL)
	{
		env = ft_split(cmd[i], '=');//==이 2개인 경우도 인식하는데 이거 처리 어떻게? => 무조건 첫번째 =으로 처리
		merror(env);
		if (check_add_value(env))
			add_flag = TRUE;
		if (incorrect_env_key(env[0]))
		{
			error_message("export", env[0], "not a valid identifier");
			continue ;
		}
		env_idx = check_listin(env[0], info);
		if (env_idx >= 0)
			add_env_value(env_idx, env, info);
		else
			add_new_env(cmd, info, i);
		free_double_string(env);
		env = 0;
	}
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
// ++export qq+=asd -> value값에 이어붙이는 로직