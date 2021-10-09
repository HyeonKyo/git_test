#include "../includes/minishell.h"

int	get_fd_will_be_stdin(t_minishell *minishell, int is_redirection)
{
	int		fd;

	fd = 0;
	if (minishell->pipex.is_here_doc)
		// fd = read_string_from_stdin(minishell);
		fd = 0;
	else if (minishell->number_of_pipeline) 
		fd = minishell->pipex.new_pipe[READ];
	else if (is_redirection)
	{
		fd = open(minishell->infile_name, O_RDWR);
		if (fd == -1)
			print_error(minishell->infile_name);
	}
	return (fd);
}

int	get_fd_will_be_stdout(t_minishell *minishell, int is_redirection)
{
	int		fd;

	fd = 1;
	if (minishell->number_of_pipeline)
		fd = minishell->pipex.old_pipe[1];
	if (is_redirection)
	{
		if (minishell->pipex.is_here_doc)
			fd = open(minishell->outfile_name, O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			fd = open(minishell->outfile_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
			print_error(minishell->outfile_name);
	}
	return (fd);
}
