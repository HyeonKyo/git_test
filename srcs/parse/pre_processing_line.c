#include "minishell.h"

/*
** =============================================================================
** parse_utils.c
** =============================================================================
*/

char	*remove_space(char *line, int start_idx)
{
	int		i;
	int		space_len;
	char	*front;
	char	*back;
	char	*new;

	if (check_type(line[start_idx]) != SPCE)
		return (line);
	i = start_idx;
	while (check_type(line[i]) == SPCE)
		i++;
	space_len = i - start_idx;
	front = (char *)malloc(sizeof(char) * (start_idx + 1));
	merror(front);
	ft_strlcpy(front, line, start_idx + 1);
	back = ft_strdup(line + i);
	merror(back);
	new = ft_strjoin(front, back);
	free(front);
	free(back);
	free(line);
	return (new);
}

void	skip_separator_not_space(char *line, int *i)
{
	t_type	type;

	while (is_separator(line[*i]))
	{
		type = check_type(line[*i]);
		if (type == SPCE || type == END)
			return ;
		(*i)++;
	}
}

char	*skip_space(char *line, int *start_idx)
{
	int		i;
	char	*new;
	t_type	type;

	i = *start_idx;
	new = ft_strdup(line);
	merror(new);
	free(line);
	while (is_separator(new[i]))
	{
		new = remove_space(new, i);
		skip_separator_not_space(new, &i);
		if (new[i] == 0)
			break ;
	}
	*start_idx = i;
	return (new);
}

char	*pre_processing(char *line, t_info *info)
{
	int		start_idx;
	int		sep_idx;
	int		end_flag;
	char	*new;

	start_idx = 0;
	sep_idx = 0;
	end_flag = FALSE;
	new = ft_strdup(line);
	free(line);
	while (TRUE)
	{
		sep_idx = find_separator(new, start_idx);
		if (!new[sep_idx])//끝까지 다본 경우 뿐만 아니라 << EOF같은 경우도 있음.
			end_flag = TRUE;
		new = arrange_quote(new, &start_idx, sep_idx, info);
		if (new == NULL)
			return (NULL);
		new = skip_space(new, &start_idx);
		if (end_flag)
			break ;
	}
	return (new);
}