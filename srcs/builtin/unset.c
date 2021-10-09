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
	free_env_list(env_list);
	info->env_list = new;
}

void	unset(char **cmd, t_info *info)
{
	int		idx;
	char	*value;

	value = cmd[1];
	idx = check_listin(value, info);
	if (idx >= 0)
		del_env_value(idx, info);
}

