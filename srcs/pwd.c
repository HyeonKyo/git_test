#include "minishell.h"

int	pwd(int *fd)
{
	char	*cur_path;

	cur_path = getcwd(NULL, 0);
	if (cur_path == NULL)
		return (error());
	ft_putendl_fd(cur_path, fd[WRITE]);
	free(cur_path);
	return (NORMAL);
}
