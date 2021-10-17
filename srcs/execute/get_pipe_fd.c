#include "minishell.h"

void	get_pipe_fd(t_info *info, int depth, int fd[])
{
	fd[READ] = STDIN_FILENO;
	fd[WRITE] = STDOUT_FILENO;
	if (depth != 0)//첫 번째 명령어는 파이프로 입력을 받는 경우가 없음.
		fd[READ] = info->pipex.pipe_fd[depth][READ];
	if (depth < (info->n_cmd - 1))//마지막 명령어는 파이프로 출력을 보내는 경우가 없음
		fd[WRITE] = info->pipex.pipe_fd[depth + 1][WRITE];
}