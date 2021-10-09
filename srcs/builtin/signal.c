#include "minishell.h"
#include "builtin.h"

void	sigint_handler(void)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	catch_signal(int signo)
{
	if (signo == SIGINT)
		sigint_handler();
	else if (signo == SIGQUIT)
		return ;
}