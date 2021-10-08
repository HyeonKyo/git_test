#include "../includes/minishell.h"

void	input_command_of_pipeline(t_minishell *minishell, int is_redirection)
{
	char	**command;
	int		fd_for_standard_in;

	fd_for_standard_in = get_fd_will_be_stdin(minishell, is_redirection);
	if (minishell->number_of_pipeline)
	{
		dup2(minishell->pipex.new_pipe[WRITE], 1);
		close(minishell->pipex.new_pipe[READ]);
		close(minishell->pipex.new_pipe[WRITE]);
	}
	command = ft_split(minishell->command[minishell->command_sequence], ' ');
	execute_input_command(minishell, command, fd_for_standard_in);
	close(fd_for_standard_in);
	free_two_dimensional(command);
}

void	output_command_of_pipeline(t_minishell *minishell, int is_redirection)
{
	char	**command;
	int		fd_for_standard_out;

	fd_for_standard_out = get_fd_will_be_stdout(minishell, is_redirection);
	dup2(minishell->pipex.new_pipe[READ], 0);
	close(minishell->pipex.new_pipe[WRITE]);
	close(minishell->pipex.new_pipe[READ]);
	command = ft_split(minishell->command[minishell->command_sequence], ' ');
	execute_output_command(minishell, command, fd_for_standard_out);
	close(fd_for_standard_out);
	free_two_dimensional(command);
}
