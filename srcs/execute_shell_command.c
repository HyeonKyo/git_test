#include "../includes/minishell.h"

static void	child_process_of_pipeline(t_info *info, int depth)
{
	int		is_redirection;
	char	**command;
	int		fd[2];

	is_redirection = 0; //리다이렉션 나중에 구현...
	info->cmd_sequence = depth - 1;
	fd[READ] = get_fd_will_be_stdin(info, is_redirection);
	fd[WRITE] = get_fd_will_be_stdout(info, is_redirection);
	command = ft_split(info->cmd[info->cmd_sequence], ' ');
	execute_command(info, command, fd);
	if (fd[READ] != STDIN_FILENO)
		close(fd[READ]);
	if (fd[WRITE] != STDOUT_FILENO)
		close(fd[WRITE]);
	free_two_dimensional(command);
}

static void	parent_process_of_pipeline(t_info *info, int depth)
{
	int		is_redirection;
	char	**command;
	int		fd[2];

	is_redirection = 0; //리다이렉션 나중에 구현...
	info->cmd_sequence = depth;
	fd[READ] = get_fd_will_be_stdin(info, is_redirection);
	fd[WRITE] = get_fd_will_be_stdout(info, is_redirection);
	command = ft_split(info->cmd[info->cmd_sequence], ' ');
	execute_command(info, command, fd);
	if (fd[READ] != STDIN_FILENO) //파일이나 파이프에서 받아온 fd 닫아주기
		close(fd[READ]);
	if (fd[WRITE] != STDOUT_FILENO) //파일이나 파이프에서 받아온 fd 닫아주기
		close(fd[WRITE]);
	free_two_dimensional(command);
}

/*execute_command*() 함수의 depth 매개변수는 파이프라인 개수를 의미*/
int	execute_shell_command(t_info *info, int depth)
{
	int		pid;
	int		exit_status_of_child;

	if (depth < 0 || (depth == 0 && info->n_pipeline))
		return (NORMAL);
	info->pipex.old_pipe[READ] = info->pipex.new_pipe[READ];
	info->pipex.old_pipe[WRITE] = info->pipex.new_pipe[WRITE];
	if (pipe(info->pipex.new_pipe) < 0)
		return (error());
	pid = fork();
	if (pid == -1)
		return (error());
	else if (pid > 0)
	{
		waitpid(pid, &exit_status_of_child, 0);
		/* number_of_pipeline이 0보다 클 경우에만 부모 프로세스에서 명령어 실행*/
		parent_process_of_pipeline(info, depth);
	}
	else if (pid == 0)
	{
		execute_shell_command(info, depth - 1);
		/* depth <= 1인 경우에만 자식 프로세스가 명령어를 실행하는 이유는
		자식 프로세스(프로세스 B)에서 재귀를 타고 재귀 탈출 조건에 맞지 않아 그대로
		 fork()를하면 자식 프로세스(프로세스 B)도 자식(프로세스 C)을 낳아 부모 프로세스가 된다.
		 프로세스 B가 명령어를 실행하고 함수가 종료되어 재귀가 끝나고
		 본 함수로 돌아오면 child_process_of_pipeline()를 실행할 것이다.
		 하지만 이미 명령어를 실행시켰기 때문에 한 번 더 child_process_of_pipeline() 함수를
		 실행하면 명령어를 중복 실행하게 되는 것이다.
		 그래서 if (depth <= 1) 즉, 제일 먼저 실행되는 명령어만
		 child_process_of_pipeline() 이 함수를 실행하는 것이다.
		*/
		if (depth <= 1 && info->n_pipeline)
			child_process_of_pipeline(info, depth);
		exit(EXIT_SUCCESS);
	}
	return (NORMAL);
}
