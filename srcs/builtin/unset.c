# include "minishell.h"

void	del_env_value(int idx, t_info *info)
{
	int		i;
	int		j;
	int		list_len;
	char	**env_list;
	char	**new;

	env_list = info->env_list;
	list_len = get_env_list_size(env_list);
	new = (char **)malloc(sizeof(char *) * list_len);
	merror(new);
	i = 0;
	j = 0;
	while (j < list_len)
	{
		if (j == idx)
		{
			j++;
			continue ;
		}
		new[i++] = env_list[j++];
	}
	new[i] = NULL;
	free_double_string(env_list);
	info->env_list = new;
}

void	unset(char **cmd, t_info *info)
{
	int		i;
	int		env_idx;
	char	*key;

	i = 0;
	while (cmd[++i] != NULL)
	{
		key = cmd[i];
		if (incorrect_env_key(key))
		{
			error_message(cmd[0], key, "not a valid identifier");
			continue ;
		}
		env_idx = check_listin(key, info);
		if (env_idx >= 0)
			del_env_value(env_idx, info);
	}
}
