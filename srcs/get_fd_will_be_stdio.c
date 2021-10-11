#include "minishell.h"

int	get_fd_will_be_stdin(t_info *info, int is_redirection)
{
	int		fd;

	fd = STDIN_FILENO;
	if (info->pipex.is_here_doc)
		// fd = read_string_from_stdin(info); /*here_doc '<<'*/
		fd = 0;
	else if (info->n_pipeline && info->cmd_sequence != 0)
	{
		fd = info->pipex.new_pipe[READ];
		close(info->pipex.new_pipe[WRITE]);
	}
		/*
		**표준 입력이 파이프인 경우**
			파이프로부터 입력을 받는 경우 무조건 new_pipe로 부터 받는다!
		*/
	else if (is_redirection)
	{
		fd = open(info->infile_name, O_RDWR);
		if (fd == -1)
			error();

	}
	return (fd);
}

int	get_fd_will_be_stdout(t_info *info, int is_redirection)
{
	int		fd;

	fd = STDOUT_FILENO;
	if (info->n_pipeline)
	{
		//맨 마지막 명령어는 파이프로 출력하지 않는다.
		if (info->cmd_sequence < info->n_pipeline)
		{
			fd = info->pipex.old_pipe[WRITE];
			close(info->pipex.old_pipe[READ]);
		}
		if (info->cmd_sequence == 0) 
		{
			fd = info->pipex.new_pipe[WRITE];
			close(info->pipex.new_pipe[READ]);
		}
		/*
		**표준 출력이 파이프인 경우**
			맨 처음 실행되는 명령어 : new_pipe로 데이터 출력
			나머지 모든 명령어 : old_pipe 데이터 출력
		*/
	}
	if (is_redirection)
	{
		if (info->pipex.is_here_doc)
			fd = open(info->outfile_name, O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			fd = open(info->outfile_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
			error();
	}
	return (fd);
}
