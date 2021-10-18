#include "minishell.h"

extern int	g_exit_code;

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

void	here_doc_handler(int signo)
{
	if (signo == SIGINT)
		g_exit_code = -424242;
}