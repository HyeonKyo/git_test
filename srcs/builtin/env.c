#include "minishell.h"

void	env(t_info *info, int *fd)
{
	t_env	*cur;

	cur = info->env_deq->head;
	while (cur != NULL)
	{
		if (cur->env_flag == TRUE)
		{
			ft_putstr_fd(cur->key, fd[WRITE]);
			ft_putstr_fd("=", fd[WRITE]);
			ft_putendl_fd(cur->value, fd[WRITE]);
		}
		cur = cur->next;
	}
}
