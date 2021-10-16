#include "minishell.h"

int	error(void)
{
	const char	*error_message;

	error_message = strerror(errno);
	ft_putendl_fd((char *)error_message, STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return (ERROR);
}

void	merror(void *addr)
{
	if (addr == NULL)
	{
		strerror(errno);
		exit(2);
	}
}

void	error_message(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}