#include "minishell.h"

void	malloc_cmd_list(t_info *info)
{
	int		cnt;
	t_lst	*cur;
	t_lst	*next;

	cnt = 0;
	cur = info->cmd_lst[info->cmd_sequence].text;
	while (cur != NULL)
	{
		cur = cur->next;
		cnt++;
	}
	info->cmd_list = (char **)malloc(sizeof(char *) * (cnt + 1));
}

void	get_cmd_list(t_info *info)
{
	int		cnt;
	t_lst	*cur;
	t_lst	*next;

	cnt = 0;
	malloc_cmd_list(info);
	merror(info->cmd_list);
	cur = info->cmd_lst[info->cmd_sequence].text;
	while (cur != NULL)
	{
		if (cur->str != NULL && cur->str[0] != '\0')
		{
			info->cmd_list[cnt] = ft_strdup(cur->str);
			cnt++;
		}
		cur = cur->next;
	}
	info->cmd_list[cnt] = NULL;
}