#include "minishell.h"

t_lst	*create_node(void)
{
	t_lst	*lst;

	lst = (t_lst *)malloc(sizeof(t_lst));
	merror(lst);
	ft_memset(lst, 0, sizeof(t_lst));
	return (lst);
}

void	link_node(char *cmd, t_lst **list)
{
	t_lst	*cur;
	t_lst	*tmp;

	cur = *list;
	tmp = create_node();
	tmp->str = ft_strdup(cmd);
	merror(tmp->str);
	if (cur == NULL)
		*list = tmp;
	else
	{
		while (cur->next)
			cur = cur->next;
		cur->next = tmp;
	}
}
