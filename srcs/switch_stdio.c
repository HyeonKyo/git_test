#include "minishell.h"

void	switch_stdio(t_info *info, int fd_stdin, int fd_stdout)
{
	//fd_stdout이 1이 아니면 표준 출력을 fd_stdout으로 변경
	if (fd_stdout != STDOUT_FILENO)
	{
		dup2(fd_stdout, 1);
		close(fd_stdout);//필요없어진 fd는 close()로 닫아준다
	}

	//fd_stdin이 0이 아니면 표준 입력을 fd_stdin으로 변경
	if (fd_stdin != STDIN_FILENO)
	{
		dup2(fd_stdin, 0);
		close(fd_stdin);//필요없어진 fd는 close()로 닫아준다
	}
}