#include "minishell.h"

void	make_pipeline(t_info *info)
{
	int	idx;

	idx = 0;
	info->pipex.pipe_fd = (int **)malloc(sizeof(int * ) * info->cmd_cnt);
	while (idx < info->cmd_cnt)
	{
		info->pipex.pipe_fd[idx] = (int *)malloc(sizeof(int) * 2);
		if (pipe(info->pipex.pipe_fd[idx]) < 0)
		{
			perror("pipe error\n");
			exit(1);
		}
		idx++;
	}
}
