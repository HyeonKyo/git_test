#include "minishell.h"

void	free_double_string(char **list)
{
	int	i;

	i = 0;
	if (list == NULL)
		return ;
	while(list[i] == NULL)
		free(list[i++]);
	free(list);
}
