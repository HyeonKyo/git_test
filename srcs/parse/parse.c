#include "minishell.h"

t_type	is_quote(char c)
{
	if (c == '\"')
		return (DQUOTE);
	else if (c == '\'')
		return (SQUOTE);
	return (NORM);
}

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (TRUE);
	return (FALSE);
}

int	check_incorrect_line(char *line)
{
	/*
	1. quote가 짝수개인지
	2. quote안이 아닌 곳에 ; \ 있는지
	3. double pipe인지
	*/
}

int	find_space_not_in_quote(char *line, int idx)
{
	int	squote_flag;
	int	dquote_flag;

	squote_flag = FALSE;
	dquote_flag = FALSE;
	while (line[idx])
	{
		if (is_quote(line[idx]) == SQUOTE)
			squote_flag ^= TRUE;
		else if (is_quote(line[idx]) == DQUOTE)
			dquote_flag ^= TRUE;
		if (!squote_flag && !dquote_flag && is_space(line[idx]))
			return (idx);
	}
	return (FALSE);
}

char	*clean_quote_before_space(char *new, int *idx)
{
	int	i;
	int	end_idx;
	int	squote_flag;
	int	dquote_flag;

	if (*idx == 0)
		return (new);
	i = 0;
	end_idx = *idx;
	squote_flag = FALSE;
	dquote_flag = FALSE;
	while (i < end_idx)
	{
		if (is_quote(new[i]) == SQUOTE)
			squote_flag ^= TRUE;
		else if (is_quote(new[i]) == DQUOTE)
			dquote_flag ^= TRUE;
	}
}

char	*arrange_quote(char *line)
{
	int		idx;
	char	*new;

	idx = 0;
	new = ft_strdup(line);
	while (idx)
	{
		new = clean_quote_before_space(new, &idx);
		new = clean_space(new, &idx);
		idx = find_space_not_in_quote(new, idx);
	}
}

char	*pre_processing(char *line)
{
	char	*cmd;

	cmd = arrange_quote(line);
	free(line);
	arrange_space(cmd);
	return (cmd);
}

int	parse_line(char *line, t_info *info)
{
	char	*cmd;

	if (check_incorrect_line(line))
		return (ERROR);
	cmd = pre_processing(line);
	make_command(cmd, info);
	return (NORMAL);
}
