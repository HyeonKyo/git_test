#include "minishell.h"

extern int	g_exit_code;

int	read_string_from_stdin(t_info *info, char *limiter)
{
	char	*str;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	while (TRUE)
	{
		str = readline(">");//readline으로 변경, ctrl + d받으면 NULL
		if (str == NULL || g_exit_code == -424242
			|| strncmp(str, limiter, ft_strlen(limiter)) == 0)
			break ;
		ft_putendl_fd(str, pipe_fd[WRITE]);//받은 문자열을 파이프에 저장
		free(str);
		str = NULL;
	}
	info->pipex.is_here_doc = 0;
	// signal(SIGINT, sig_handler);
	close(pipe_fd[WRITE]);
	return (pipe_fd[READ]);//문자열이 저장된 파이프의 fd를 반환
}

int	here_doc(t_info *info, char *limiter, int fd[])
{
	info->pipex.is_here_doc = 1;
	// signal(SIGINT, here_doc_handler);
	fd[READ] = read_string_from_stdin(info, limiter);//파이프의 fd 리턴, 에러 시 -1 리턴
	if (fd[READ] == -1)
		return (ERROR);
	return (NORMAL);
}