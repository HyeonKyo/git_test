#include "minishell.h"

int	parse_line(char *line, t_info *info)
{
	char	*new_line;

	if (check_incorrect_line(line))
		return (ERROR);
	new_line = pre_processing(line, info);
	make_command(new_line, info);
	return (NORMAL);
}