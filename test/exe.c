#include "minishell.h"

int main(int ac, char **av, char **ev)
{
	char *str[4];

	str[0] = " echo  ";
	str[1] = " -n";
	str[2] = "asd ";
	str[3] = NULL;
	execve("/bin/echo", str, ev);
	ft_putendl_fd("bash: command is not found!", STDOUT_FILENO);
	return 0;
}
