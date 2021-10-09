#include "minishell.h"

static void	child_process_of_pipeline(t_minishell *minishell, int depth)
{
	int		is_redirection;

	is_redirection = 0; /*리다이렉션 나중에 구현...*/
	// if (depth == 0 && !(minishell->pipex.is_here_doc))
	// 	is_redirection = 1;
	minishell->command_sequence = depth - 1;
	if (!minishell->number_of_pipeline)
		minishell->command_sequence = 0; //파이프라인이 없으면 실행할 명령어가 한 개니까 0으로 초기화
	input_command_of_pipeline(minishell, is_redirection);
	exit(EXIT_SUCCESS);
}

static void	parent_process_of_pipeline(t_minishell *minishell, int depth)
{
	int		is_redirection;

	is_redirection = 0; /*리다이렉션 나중에 구현...*/
	// if (depth == (minishell->command_total_number - 2))
	// 	is_redirection = 1;
	minishell->command_sequence = depth;
	output_command_of_pipeline(minishell, is_redirection);
}

/*execute_command*() 함수의 depth 매개변수는 파이프라인 개수를 의미*/
void	execute_shell_command(t_minishell *minishell, int depth)
{
	int		pid;
	int		exit_status_of_child;

	if (depth < 0 || (depth == 0 && minishell->number_of_pipeline))
		return ;
	minishell->pipex.old_pipe[0] = minishell->pipex.new_pipe[0];
	minishell->pipex.old_pipe[1] = minishell->pipex.new_pipe[1];
	if (pipe(minishell->pipex.new_pipe) < 0)
		print_error("pipe error");
	pid = fork();
	if (pid == -1)
		print_error("fork error");
	else if (pid > 0)
	{
		waitpid(pid, &exit_status_of_child, WNOHANG);
		/* number_of_pipeline이 0보다 클 경우에만 부모 프로세스에서 명령어 실행*/
		if (minishell->number_of_pipeline)
			parent_process_of_pipeline(minishell, depth);
	}
	else if (pid == 0)
	{
		execute_shell_command(minishell, depth - 1);
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
		if (depth <= 1)
			child_process_of_pipeline(minishell, depth);
	}
}
