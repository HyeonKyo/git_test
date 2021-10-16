#include "minishell.h"

void	switch_stdio(t_info *info, int fd_stdin, int fd_stdout)
{
	if (dup2(fd_stdout, WRITE) == -1)//** 현교 : 변수 WRITE가 의미하는게 STDOUT이라면 변수명 STDOUT_FILENO로 변경하는게 좋을 듯?
		error();
	if (dup2(fd_stdin, READ) == -1)
		error();
	close_pipeline(info);
}