#include "minishell.h"

extern int	g_exit_code;

int	read_string_from_stdin(t_info *info, char *limiter)
{
	char	*str;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	while (1)
	{
		if (g_exit_code == -424242)
		{
			printf("@@exit flag!@@\n");
			break ;
		}
		if (get_next_line(0, &str))//gnl 함수로 표준입력 받기
		{
			if (strncmp(str, limiter, ft_strlen(limiter)) == 0)//사용자가 limiter 입력하면 break
				break ;
			ft_putstr_fd(str, pipe_fd[WRITE]);//받은 문자열을 파이프에 저장
			ft_putstr_fd("\n", pipe_fd[WRITE]);
			free(str);
		}
		else
			break ;
	}
	info->pipex.is_here_doc = 0;
	signal(SIGINT, sig_handler);
	close(pipe_fd[WRITE]);
	return (pipe_fd[READ]);//문자열이 저장된 파이프의 fd를 반환
}

int	here_doc(t_info *info, char *limiter, int fd[])
{
	info->pipex.is_here_doc = 1;
	signal(SIGINT, here_doc_handler);
	fd[READ] = read_string_from_stdin(info, limiter);//파이프의 fd 리턴, 에러 시 -1 리턴
	if (fd[READ] == -1)
		return (ERROR);
	return (NORMAL);
}