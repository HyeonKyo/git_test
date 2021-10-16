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

int	redirection(t_info *info, int fd[])
{
	t_lst	*cur;
	t_lst	*next;
	char	**redirection;

	cur = info->cmd_lst[info->cmd_sequence].redi;
	while (cur != NULL)
	{
		redirection = ft_split(cur->str, '\"');
		if (cur->str[0] == '<')
		{
			if (input_redirection(info, redirection[1], fd))
				return (ERROR);
		}
		else if (cur->str[0] == '>')
		{
			if (output_redirection(info, redirection[1], fd))
				return (ERROR);
		}
		next = cur->next;
		cur = next;
	}
	return (NORMAL);
}