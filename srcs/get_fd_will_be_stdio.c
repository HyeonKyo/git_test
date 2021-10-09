#include "minishell.h"

int	get_fd_will_be_stdin(t_info *info, int is_redirection)
{
	int		fd;

	fd = 0;
	if (info->pipex.is_here_doc)
		// fd = read_string_from_stdin(info); /*here_doc '<<'*/
		fd = 0;
	else if (info->n_pipeline)
		fd = info->pipex.new_pipe[READ];
	else if (is_redirection)
	{
		fd = open(info->infile_name, O_RDWR);
		if (fd == -1)
			print_error(info->infile_name);
	}
	return (fd);
}

int	get_fd_will_be_stdout(t_info *info, int is_redirection)
{
	int		fd;

	fd = 1;
	if (info->n_pipeline)
		fd = info->pipex.old_pipe[1];
	if (is_redirection)
	{
		if (info->pipex.is_here_doc)
			fd = open(info->outfile_name, O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			fd = open(info->outfile_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
			print_error(info->outfile_name);
	}
	return (fd);
}
