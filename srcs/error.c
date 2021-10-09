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
