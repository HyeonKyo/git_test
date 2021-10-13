#include "minishell.h"

void	execute_command(t_info *info, int depth)
{
	int	status;

	info->cmd_sequence = depth;	
	info->pipex.pid[depth] = fork();
	if (info->pipex.pid[depth] == -1)
		error();
	if (info->pipex.pid[depth] > 0)
	{
		if (depth == info->cmd_cnt - 1)//마지막 명령어이면 if문 진입
		{
			close_pipeline(info);//부모에서 파이프라인 다 닫음(자식엔 영향 없음)
			while (wait(&status) != -1)//모든 자식 프로세스가 끝날 때 까지 대기
			{
				;
			}
			return ;//마지막 명령어이면 재귀 그만 돌고 리턴
		}
		execute_command(info, depth + 1);//부모에서 execute_command() 함수 재귀 호출
	}
	if (info->pipex.pid[depth] == 0)
	{
		execute_execve_function(info, depth);
		exit(1);
	}
}

void	execute_command_main(t_info *info, int depth)
{
	make_pipeline(info);//파이프라인 생성
	info->pipex.pid = (int *)malloc(sizeof(int) * info->cmd_cnt);
	info->cmd_sequence = 0;
	if (is_builtin_command(info) && info->cmd_cnt == 1)
	{
		execute_execve_function(info, 0);
		return ;
	}
	execute_command(info, depth);
}