#include "minishell.h"

void	make_pipeline(t_info *info)
{
	int		idx;
	char	*error_message;

	idx = 0;
	info->pipex.pipe_fd = (int **)malloc(sizeof(int * ) * info->n_cmd);
	while (idx < info->n_cmd)
	{
		info->pipex.pipe_fd[idx] = (int *)malloc(sizeof(int) * 2);
		if (pipe(info->pipex.pipe_fd[idx]) < 0)
		{
			error_message = strerror(errno);
			ft_putendl_fd((char *)error_message, STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		idx++;
	}
}
