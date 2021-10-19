#include "minishell.h"

void	init_quote_data(t_quote *data)
{
	data->squote_flag = 0;
	data->dquote_flag = 0;
	data->squote_cnt = 0;
	data->dquote_cnt = 0;
}

static int	check_redirection(char *line, int i)
{
	int		redi_cnt;
	t_type	type;

	redi_cnt = 1;
	type = is_redirection(line[i]);
	while (type == is_redirection(line[i + redi_cnt]) && line[i + redi_cnt])
		redi_cnt++;
	if (redi_cnt <= 2 && !is_redirection(line[i + redi_cnt]))
		return (TRUE);
	return (FALSE);
}

static int	check_incorrect_case(char *line, int i)
{
	if ((line[i] == ';' || line[i] == '\\'))
		return (TRUE);
	if (check_type(line[i]) == PIPE && check_type(line[i + 1] == PIPE))
		return (TRUE);
	if (is_redirection(line[i]) && check_redirection(line, i))
		return (TRUE);
	return (FALSE);
	//; or \
	//더블 파이프
	//리다이렉션 <> >< or >>>처럼 3개 이상인 경우
	//체크
}

int	check_incorrect_line(char *line)
{
	int		i;
	t_quote	data;

	init_quote_data(&data);
	i = -1;
	while (line[++i])
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
		if (!data.squote_flag && !data.dquote_flag)
			if (check_incorrect_case(line, i))
				return (TRUE);
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