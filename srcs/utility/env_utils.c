#include "minishell.h"

char	**env_split(char *str)
{
	int		key_len;
	char	*sep;
	char	**env;

	env = (char **)malloc(sizeof(char *) * 3);
	merror(env);
	sep = ft_strchr(str, '=');
	key_len = sep - str + 1;
	if (sep == NULL)
		key_len = ft_strlen(str) + 1;
	env[KEY] = (char *)malloc(sizeof(char) * key_len);
	merror(env[KEY]);
	ft_strlcpy(env[KEY], str, key_len);
	if (sep)
		env[VALUE] = ft_strdup(sep + 1);
	else
	{
		env[VALUE] = (char *)malloc(sizeof(char));
		env[VALUE][0] = '\0';
	}
	merror(env[VALUE]);
	env[2] = NULL;
	return (env);
	//처음 만나는 =으로 2등분
	//만약 export qq만 오면
	//env 2차원 배열의 env[1]은 1만큼 할당 후 '\0'을 넣어줌.
	//env[2] == NULL
}

char	*get_env_value(char *key, t_info *info)//key문자열의 복사본 리턴, 없으면 null
{
	t_env	*cur;
	char	*ret;

	cur = check_listin(key, info);
	if (cur == NULL)
		return(NULL);
	ret = ft_strdup(cur->value);
	merror(ret);
	return (ret);
}

int	is_register_variable(char *cmd)
{
	if (ft_strchr(cmd, '=') != NULL)
		return (TRUE);
	return(FALSE);
}

void	register_variable(char *cmd, t_info *info)
{
	int		flag;
	char	*export_cmd[3];
	char	**env;
	t_env	*end;
	t_env	*tmp;

	flag = FALSE;
	export_cmd[0] = "export";
	export_cmd[1] = ft_strdup(cmd);
	merror(export_cmd[1]);
	export_cmd[2] = NULL;
	env = env_split(cmd);
	if (check_listin(env[KEY], info))
		flag = TRUE;
	free_double_string(env);
	export(export_cmd, info);
	if (flag == FALSE)//이미 있는 변수의 값만 바꾸는 경우
		info->env_deq->last->env_flag = FALSE;
	free(export_cmd[1]);
}