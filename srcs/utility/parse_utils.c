#include "minishell.h"

int	is_special(char c)
{
	if (c <= 47 || (c >= 58 && c <= 64) || (c <= 91 && c >= 96) || c >= 123)
		return (TRUE);
	return (FALSE);
}

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (TRUE);
	return (FALSE);
}

t_type	is_redirection(char c)
{
	if (c == '<')
		return (LRDI);
	else if (c == '>')
		return (RRDI);
	// else if (c == '<<')
	// 	return (DRRDI);
	// else if (c == '>>')
	// 	return (DLRDI);
	return (FALSE);
}

t_type	check_type(char c)
{
	if (c == '\"')
		return (DQUOTE);
	else if (c == '\'')
		return (SQUOTE);
	else if (is_space(c))
		return (SPCE);
	else if (c == '|')
		return (PIPE);
	else if (c == '\0')
		return (END);
	else if (is_redirection(c))
		return (is_redirection(c));
	else if (is_special(c))
		return (SPEC);
	return (NORM);
}

int	is_separator(char c)
{
	t_type	type;

	type = check_type(c);
	if (type == SPCE || type == PIPE || type == END
		|| type == RRDI || type == LRDI)
		return (TRUE);
	return (FALSE);
}