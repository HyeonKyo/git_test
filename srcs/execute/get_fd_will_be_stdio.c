#include "minishell.h"

int	get_fd_will_be_stdin(t_info *info, int depth, int is_redirection)
{
	int		fd;

	fd = STDIN_FILENO;
	if (info->pipex.is_here_doc)
		// fd = read_string_from_stdin(info); /*here_doc '<<'*/
		fd = 0;
	else if (info->n_pipe && info->cmd_sequence != 0)
	{
		fd = info->pipex.pipe_fd[depth][READ];
	}
	else if (is_redirection)
	{
		fd = open(info->infile_name, O_RDWR);
		if (fd == -1)
			error();
	}
	return (fd);
}

int	get_fd_will_be_stdout(t_info *info, int depth, int is_redirection)
{
	int		fd;

	fd = STDOUT_FILENO;
	if (info->n_pipe)
	{
		if (depth < (info->n_cmd - 1))
			fd = info->pipex.pipe_fd[depth + 1][WRITE];
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
