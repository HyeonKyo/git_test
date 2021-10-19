#include "minishell.h"

/*
** =============================================================================
** check_line.c
** =============================================================================
*/

void	init_quote_data(t_quote *data)
{
	data->squote_flag = 0;
	data->dquote_flag = 0;
	data->squote_cnt = 0;
	data->dquote_cnt = 0;
}

int	check_incorrect_line(char *line)
{
	int		i;
	t_quote	data;

	init_quote_data(&data);
	i = 0;
	while (line[i])
	{
		if (check_type(line[i]) == SQUOTE && data.dquote_flag == FALSE)
		{
			data.squote_cnt++;
			data.squote_flag ^= TRUE;
		}
		else if (check_type(line[i]) == DQUOTE && data.squote_flag == FALSE)
		{
			data.dquote_cnt++;
			data.dquote_flag ^= TRUE;
		}
		if (!data.squote_flag && !data.dquote_flag &&
			(line[i] == ';' || line[i] == '\\'))
			return (TRUE);
		//리다이렉션 체크 추가
		//1. <>처럼 다른 게 연속으로 올 떄
		//2. >>> 처럼 3개 이상이 연속으로 올때
		//double pipe인지
		//맨처음line[0]이 separator인지
		i++;
	}
	if ((data.squote_cnt & ISODD) || (data.dquote_cnt & ISODD))
		return (TRUE);
	return (FALSE);
	/*
	1. quote가 짝수개인지
	2. quote안이 아닌 곳에 ; \ 있는지
	3. double pipe인지
	*/
}