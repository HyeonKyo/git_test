#include "minishell.h"

void	free_two_dimensional(char **two_dimensional)
{
	int	idx;

	idx = 0;
	while (two_dimensional[idx])
	{
		if (two_dimensional[idx])
			free(two_dimensional[idx]);
		idx++;
	}
	if (two_dimensional)
		free(two_dimensional);
}
