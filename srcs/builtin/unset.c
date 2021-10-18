# include "minishell.h"

void	del_env_variable(t_env *cur, t_info *info)
{
	t_env	*pre;
	t_env	*next;
	t_env	*del;

	pre = cur->prev;
	next = cur->next;
	if (info->env_deq->head == cur)
		info->env_deq->head = info->env_deq->head->next;
	if (info->env_deq->last == cur)
		info->env_deq->last = info->env_deq->last->prev;
	link_env_node(pre, next);
	free(cur);
	info->env_deq->size -= 1;
	free_double_string(info->env_list);
	make_env_double_string(info);
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
