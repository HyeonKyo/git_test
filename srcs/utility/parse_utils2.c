#include "minishell.h"

int	find_separator(char *line, int idx)
{
	int		squote_flag;
	int		dquote_flag;
	t_type	type;

	squote_flag = FALSE;
	dquote_flag = FALSE;
	while (line[idx])
	{
		type = check_type(line[idx]);
		if (type == SQUOTE && dquote_flag == FALSE)
			squote_flag ^= TRUE;
		else if (type == DQUOTE && squote_flag == FALSE)
			dquote_flag ^= TRUE;
		if (!squote_flag && !dquote_flag && (type == SPCE || type == PIPE
			|| is_redirection(line[idx])))
			break ;
		idx++;
	}
	return (idx);
}

static int	find_quote_idx(char *cmd, int *idx)
{
	/*
	끝까지 가면 FALSE리턴
	1. 처음에 quote만나면 idx++하면서 flag세우기
	2. flag닫힐떄까지 쭉 넘김
	3. 닫힌 flag의("의)idx를 리턴
	=> 이 로직으로 하기 위해 파싱 작업에서 일반문자열 안에 "가 있으면 '로 양끝을 묶고
		그 외의 경우는 "로 양끝을 묶도록 작업해줌.
	*/
	while (cmd[*idx])
	{
		if (cmd[*idx] == '\"')
		{
			(*idx)++;
			while(cmd[*idx] != '\"' && cmd[*idx] != 0)
				(*idx)++;
			break ;
		}
		else if (cmd[*idx] == '\'')
		{
			(*idx)++;
			while(cmd[*idx] != '\'' && cmd[*idx] != 0)
				(*idx)++;
			break ;
		}
		else
			(*idx)++;
	}
	if (cmd[*idx] == '\"' || cmd[*idx] == '\'')
		return (TRUE);
	return (FALSE);
}

static char	**create_cmd_lst(char *cmd)
{
	int		i;
	int		str_cnt;
	char	**cmd_lst;

	str_cnt = 0;
	i = 0;
	while (find_quote_idx(cmd, &i))
	{
		str_cnt++;
		i++;
	}
	cmd_lst = (char **)malloc(sizeof(char *) * (str_cnt + 1));
	merror(cmd_lst);
	cmd_lst[str_cnt] = NULL;
	return (cmd_lst);
}

char	**quote_split(char *cmd)
{
	int		i;
	int		quote_idx;
	int		pre_idx;
	char	**cmd_lst;

	cmd_lst = create_cmd_lst(cmd);
	// >>"텍스트" -> 처음부터 뒤의 "인덱스까지 파싱하기 때문에
	i = 0;
	quote_idx = 0;
	pre_idx = 0;
	while (cmd_lst[i])
	{
		find_quote_idx(cmd, &quote_idx);
		cmd_lst[i] = (char *)malloc(sizeof(char) * quote_idx - pre_idx);
		merror(cmd_lst[i]);
		if ((cmd[quote_idx] == '\"' && cmd[pre_idx] != '\"')
			|| (cmd[quote_idx] == '\'' && cmd[pre_idx] != '\''))
			pre_idx--;
		ft_strlcpy(cmd_lst[i], cmd + pre_idx + 1, quote_idx - pre_idx);
		pre_idx = ++quote_idx;
		i++;
	}
	return (cmd_lst);
}