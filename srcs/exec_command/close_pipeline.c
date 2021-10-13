#include "minishell.h"

void	close_pipeline(t_info *info)
{
	int	idx;

	idx = 0;
	while (idx < info->cmd_cnt)
	{
		close(info->pipex.pipe_fd[idx][READ]);
		close(info->pipex.pipe_fd[idx][WRITE]);
		idx++;
	}
}
