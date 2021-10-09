#include "minishell.h"

void	env(t_info *info, int *fd)
{
	int	i;

	i = 0;
	while (info->env_list[i] != NULL)
		ft_putendl_fd(info->env_list[i++], fd[WRITE]);
}
