# include "minishell.h"

void	del_env_variable(t_env *cur, t_info *info)
{
	t_env	*pre;
	t_env	*next;
	t_env	*del;

	pre = cur->prev;
	next = cur->next;
	link_env_node(pre, next);
	free(cur);
	info->env_deq->size -= 1;
}

void	unset(char **cmd, t_info *info)
{
	int		i;
	char	*key;
	t_env	*cur;

	i = 0;
	while (cmd[++i] != NULL)
	{
		key = cmd[i];
		if (incorrect_env_key(key))
		{
			error_message(cmd[0], key, "not a valid identifier");
			continue ;
		}
		cur = check_listin(key, info);
		if (cur)
			del_env_variable(cur, info);
	}
}
