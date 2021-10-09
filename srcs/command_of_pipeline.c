#include "minishell.h"

void	input_command_of_pipeline(t_info *info, int is_redirection)
{
	char	**command;
	int		fd_for_standard_in;

	fd_for_standard_in = get_fd_will_be_stdin(info, is_redirection);
	if (info->n_pipeline)
	{
		dup2(info->pipex.new_pipe[WRITE], 1);
		close(info->pipex.new_pipe[READ]);
		close(info->pipex.new_pipe[WRITE]);
	}
	command = ft_split(info->cmd[info->cmd_sequence], ' ');
	execute_input_command(info, command, fd_for_standard_in);
	close(fd_for_standard_in);
	free_two_dimensional(command);
}

void	output_command_of_pipeline(t_info *info, int is_redirection)
{
	char	**command;
	int		fd_for_standard_out;

	fd_for_standard_out = get_fd_will_be_stdout(info, is_redirection);
	dup2(info->pipex.new_pipe[READ], 0);
	close(info->pipex.new_pipe[WRITE]);
	close(info->pipex.new_pipe[READ]);
	command = ft_split(info->cmd[info->cmd_sequence], ' ');
	execute_output_command(info, command, fd_for_standard_out);
	close(fd_for_standard_out);
	free_two_dimensional(command);
}
