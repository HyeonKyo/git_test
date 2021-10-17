#include "minishell.h"

void	switch_stdio(t_info *info, int fd_stdin, int fd_stdout)
{
	if (dup2(fd_stdout, STDOUT_FILENO) == -1)
		error();
	if (dup2(fd_stdin, STDIN_FILENO) == -1)
		error();
	close_pipeline(info);
}