#include "minishell.h"

void	sigint_handler(void)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_handler(int signo)
{
	if (signo == SIGINT)
		sigint_handler();
	else if (signo == SIGQUIT)
		return ;
}