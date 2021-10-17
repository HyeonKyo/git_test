# include "minishell.h"

void	save_env_values(t_info *info, char **envp)
{
	int			size;
	t_env		*new;
	t_env		*cur;
	t_env_deq	*deq;

	new = make_env_list(envp);
	deq = (t_env_deq *)malloc(sizeof(t_env_deq));
	merror(deq);
	deq->head = new;
	cur = new;
	size = 0;
	while (cur->next != NULL)
	{
		size++;
		cur = cur->next;
	}
	deq->size = size;
	deq->last = cur;
	info->env_deq = deq;
}
