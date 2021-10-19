#include "minishell.h"

int	input_redirection(t_info *info, char *infile, int fd[])
{
	fd[READ] = open(infile, O_RDWR);
	if (fd[READ] == -1)
		return (ERROR);
	return (NORMAL);
}

int	output_redirection(t_info *info, char *outfile, int fd[])
{
	fd[WRITE] = open(outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd[WRITE] == -1)
		return (ERROR);
	return (NORMAL);
}

int	d_output_redirection(t_info *info, char *outfile, int fd[])
{
	fd[WRITE] = open(outfile, O_CREAT | O_RDWR | O_APPEND, 0644);//">>" 일 때는 open() 함수 옵션에 O_APPEND
	if (fd[WRITE] == -1)
		return (ERROR);
	return (NORMAL);
}

int	redirection(t_info *info, int fd[])
{
	t_lst	*cur;
	t_lst	*next;
	int		reval;
	char	**redi;

	reval = NORMAL;
	cur = info->cmd_lst[info->cmd_sequence].redi;
	while (cur != NULL)
	{
		redi = ft_split(cur->str, '\"');
		if (!ft_strncmp(redi[0], "<<", 2))
			reval = here_doc(info, redi[1], fd);
		else if (!ft_strncmp(redi[0], ">>", 2))
			reval = d_output_redirection(info, redi[1], fd);
		else if (redi[0][0] == '<')
			reval = input_redirection(info, redi[1], fd);
		else if (redi[0][0] == '>')
			reval = output_redirection(info, redi[1], fd);
		if (reval)
			return (ERROR);
		cur = cur->next;
	}
	return (NORMAL);
}
