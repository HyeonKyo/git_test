#include "minishell.h"

/*
** =============================================================================
** parse_utils.c
** =============================================================================
*/


int	is_spacial(char c)
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
	// else if (is_special(c))
	// 	return (SPEC);
	else if (c == '$')
		return (DOLR);
	else if (c == '\0')
		return (END);
	else if (is_redirection(c))
		return (is_redirection(c));
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

/*
** =============================================================================
** quote_handling.c
** =============================================================================
*/

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

char	**divide_by_separator(char *line, int start_idx, int sep_idx)
{
	char	**str;
	int		front_len;
	int		mid_len;
	int		back_len;

	str = (char **)malloc(sizeof(char *) * 3);
	merror(str);
	front_len = start_idx + 1;
	mid_len = sep_idx - start_idx + 1;
	back_len = ft_strlen(line) - sep_idx + 1;
	str[FRONT] = (char *)malloc(sizeof(char) * front_len);
	merror(str[FRONT]);
	str[MID] = (char *)malloc(sizeof(char) * mid_len);
	merror(str[MID]);
	str[BACK] = (char *)malloc(sizeof(char) * back_len);
	merror(str[BACK]);
	ft_strlcpy(str[FRONT], line, front_len);
	ft_strlcpy(str[MID], line + start_idx, mid_len);
	ft_strlcpy(str[BACK], line + sep_idx, back_len);
	free(line);
	return (str);
}

char	*make_buf(void)
{
	char	*buf;

	buf = (char *)malloc(sizeof(char) * BUF_SIZE);//나중에 디파인 상수로 변경
	merror(buf);
	return (buf);
}

void	replace_env_value(char **origin, int *i, t_info *info)
{
	t_type	type;
	int		key_len;
	int		env_idx;
	char	*env_key;
	char	*env_value;
	char	*front_str;
	char	*rest_str;
	char	*new;

	//parse_env_key();
	//get_env_value();
	//free(env_key);
	//origin i부터 대체해야함.
	*i += 1;
	type = check_type((*origin)[*i]);
	key_len = 1;
	while (type == NORM)
	{
		type = check_type((*origin)[*i + key_len]);
		key_len++;
	}
	env_key = (char *)malloc(sizeof(char) * key_len);
	merror(env_key);
	ft_strlcpy(env_key, (*origin + *i), key_len);//"aa$"같은 경우 테스트 해보기
	env_value = get_env_value(env_key, info);
	free(env_key);
	front_str = (char *)malloc(sizeof(char) * *i);
	merror(front_str);
	ft_strlcpy(front_str, *origin, *i);
	rest_str = ft_strdup(*origin + *i + key_len - 1);
	merror(rest_str);
	new = ft_strjoin(front_str, env_value);
	free(*origin);
	*i -= 1;
	*origin = ft_strjoin(new, rest_str);
	free(front_str);
	free(env_value);
	free(rest_str);
	free(new);
}

char	choose_quote_type(char *buf, int buf_len)
{
	int	i;

	i = 0;
	while (i < buf_len)
	{
		if (buf[i++] == '\"')
			return ('\'');
	}
	return ('\"');
}

char	*make_new_string(char *buf, int buf_len)
{
	char	*str;
	char	quote;

	str = (char *)malloc(sizeof(char) * (buf_len + 3));//양쪽에 ""넣어줄 공간까지 추가
	merror(str);
	quote = choose_quote_type(buf, buf_len);
	str[0] = quote;
	ft_strlcpy(str + 1, buf, buf_len + 1);
	str[buf_len + 1] = quote;
	str[buf_len + 2] = '\0';
	free(buf);
	return (str);
}

char	*fillin_buf(char *buf, char *origin, t_info *info)
{
	int		i;
	int		j;
	t_type	type;

	i = 0;
	j = 0;
	while (origin[i])
	{
		type = check_type(origin[i]);
		if (type == DQUOTE || type == SQUOTE)
		{
			i++;
			if (type == SQUOTE)
			{
				type = check_type(origin[i]);
				while (type != SQUOTE && origin[i])
				{
					buf[j++] = origin[i++];
					type = check_type(origin[i]);
				}
			}
			else
			{
				type = check_type(origin[i]);
				while (type != DQUOTE && origin[i])
				{
					if (type == DOLR)
					{
						replace_env_value(&origin, &i, info);
						type = check_type(origin[i]);
						continue ;
					}
					buf[j++] = origin[i++];
					type = check_type(origin[i]);
				}
			}
			i++;
		}
		else
			buf[j++] = origin[i++];
	}
	return (make_new_string(buf, j));
}

char	*make_arrange_string(char *mid, char **divide, int *start_idx)
{
	char	*tmp;
	char	*new;

	tmp = ft_strjoin(divide[FRONT], mid);
	merror(tmp);
	free(mid);
	*start_idx = (int)ft_strlen(tmp);
	new = ft_strjoin(tmp, divide[BACK]);
	merror(new);
	free(tmp);
	return (new);
}

char	*clear_quote(char *line, int *start_idx, int sep_idx, t_info *info)
{
	/*
	1. start, sep인덱스를 구분자로 3개의 문자열로 쪼개기
	2. 넉넉한 버퍼 할당. -> 기준 세우기
	3. 가운데 문자열을 환경 변수 처리, quote처리 등 해서 버퍼 채우기
	4. 채워진 버퍼 + 스페이스 + \0만큼 재할당후 문자열 만들어주고 버퍼 프리
	5. 재할당 문자열과 기존 문자열 join
	6. start인덱스를 구분자의 인덱스 위치로 변경해줌.
	*/
	char	*buf;
	char	*new;
	char	**divide;

	divide = divide_by_separator(line, *start_idx, sep_idx);
	buf = make_buf();
	new = fillin_buf(buf, divide[MID], info);
	new = make_arrange_string(new, divide, start_idx);
	free_double_string(divide);
	return (new);
}

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
		new = clear_quote(new, &start_idx, sep_idx, info);
		if (new == NULL)
			return (NULL);
		new = skip_space(new, &start_idx);
		if (end_flag)
			break ;
	}
	return (new);
}

int	count_command(char *line)
{
	int sep_idx;
	int	sep_cnt;

	sep_idx = 0;
	sep_cnt = 0;
	sep_idx = find_separator(line, sep_idx);
	while (line[sep_idx])
	{
		while (line[sep_idx] && check_type(line[sep_idx]) != PIPE)
		{
			sep_idx++;
			sep_idx = find_separator(line, sep_idx);
		}
		if (!line[sep_idx])
			break ;
		sep_cnt++;
		sep_idx++;
		sep_idx = find_separator(line, sep_idx);
	}
	return (sep_cnt + 1);
}

char	**divide_by_command(char *line, t_info *info)
{
	int		i;
	int		pre_idx;
	int		cur_idx;
	char	**cmd;

	info->n_cmd = count_command(line);
	cmd = (char **)malloc(sizeof(char *) * (info->n_cmd + 1));
	merror(cmd);
	ft_memset(cmd, 0, sizeof(char *) * (info->n_cmd + 1));
	i = 0;
	pre_idx = 0;
	cur_idx = find_separator(line, pre_idx);
	while (i < info->n_cmd)
	{
		while (check_type(line[cur_idx]) != PIPE && line[cur_idx])
		{
			if (pre_idx != cur_idx || is_separator(line[cur_idx]))
				cur_idx++;
			cur_idx = find_separator(line, cur_idx);
		}
		cmd[i] = (char *)malloc(sizeof(char) * (cur_idx - pre_idx + 1));
		merror(cmd[i]);
		ft_strlcpy(cmd[i], line + pre_idx, cur_idx - pre_idx + 1);
		pre_idx = ++cur_idx;
		i++;
	}
	free(line);
	return (cmd);
}

void	count_pipeline(char **cmd, t_info *info)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
		i++;
	info->n_pipe = i - 1;
}

int	check_redirection(char *cmd)
{
	if (is_redirection(cmd[0]))
		return (TRUE);
	return (FALSE);
}

int	find_quote_idx(char *cmd, int *idx)
{
	/*
	끝까지 가면 FALSE리턴
	1. 처음에 quote만나면 idx++하면서 flag세우기
	2. flag닫힐떄까지 쭉 넘김
	3. 닫힌 flag의("의)idx를 리턴
	=> 이 로직으로 하기 위해 파싱 작업에서 일반문자열 안에 "가 있으면 '로 양끝을 묶고
		그 외의 경우는 "로 양끝을 묶도록 작업해줌.
	*/
	t_quote	data;

	init_quote_data(&data);
	while (cmd[*idx])
	{
		if (cmd[*idx] == '\"' && data.squote_flag == FALSE)
		{
			data.dquote_flag ^= TRUE;
			(*idx)++;
			while(cmd[*idx] != '\"' && cmd[*idx] != 0)
				(*idx)++;
			if (cmd[*idx] == '\"')
				return (TRUE);
			return (FALSE);
		}
		if (cmd[*idx] == '\'' && data.dquote_flag == FALSE)
		{
			data.squote_flag ^= TRUE;
			(*idx)++;
			while(cmd[*idx] != '\'' && cmd[*idx] != 0)
				(*idx)++;
			if (cmd[*idx] == '\'')
				return (TRUE);
			return (FALSE);
		}
		(*idx)++;
	}
	return (FALSE);
}

char	**quote_split(char *cmd)
{
	int		i;
	int		quote_idx;
	int		pre_idx;
	int		str_cnt;
	char	**cmd_lst;
	t_quote	data;

	init_quote_data(&data);
	str_cnt = 0;
	i = 0;
	//make_cmd_lst
	while (find_quote_idx(cmd, &i))
	{
		str_cnt++;
		i++;
	}
	cmd_lst = (char **)malloc(sizeof(char *) * (str_cnt + 1));
	merror(cmd_lst);
	cmd_lst[str_cnt] = NULL;
	//--
	// >>"텍스트" -> 처음부터 뒤의 "인덱스까지 파싱하기 때문에
	i = 0;
	quote_idx = 0;
	pre_idx = 0;
	while (i < str_cnt)
	{
		find_quote_idx(cmd, &quote_idx);
		cmd_lst[i] = (char *)malloc(sizeof(char) * quote_idx - pre_idx);
		merror(cmd_lst[i]);
		if ((cmd[quote_idx] == '\"' && cmd[pre_idx] != '\"')
			|| (cmd[quote_idx] == '\'' && cmd[pre_idx] != '\''))
			pre_idx--;
		ft_strlcpy(cmd_lst[i], cmd + pre_idx + 1, quote_idx - pre_idx);
		//cmd_lst[i] = replace_quote_by_space(cmd_lst[i]);
		pre_idx = ++quote_idx;
		i++;
	}
	return (cmd_lst);
}

void	make_command_array(char **cmd, t_info *info)
{
	int		i;
	int		j;
	char	**split_cmd;
	t_cmd	*cmd_lst;

	cmd_lst = (t_cmd *)malloc(sizeof(t_cmd) * info->n_cmd);
	merror(cmd_lst);
	ft_memset(cmd_lst, 0, sizeof(t_cmd) * info->n_cmd);
	i = 0;
	while (i < info->n_cmd)
	{
		/*
		1. cmd[i]를 "로 스필릿(일반 문자 "는 스킵하는 로직 추가)
		2. 문자열 하나씩 봄.
		3. check_redirect를 함.
		4. redirect가 아니면 그 문자열 그대로 노드 만들어서 저장 후 text에 저장
		5. redirect면 redirect + 문자열 조합이 되어서 파싱되어있으므로
			그대로 노드 만들어서 redi리스트에 저장
		*/
		split_cmd = quote_split(cmd[i]);
		j = 0;
		while (split_cmd[j] != NULL)
		{			//리다이렉션 문자가 일반문자인지 아닌지 판별하는 로직 추가해야함.
			if (check_redirection(split_cmd[j]))
				link_node(split_cmd[j], &(cmd_lst[i].redi));
			else
				link_node(split_cmd[j], &(cmd_lst[i].text));
			j++;
		}
		free_double_string(split_cmd);
		i++;
	}
	info->cmd_lst = cmd_lst;
}

void	make_command(char *line, t_info *info)
{
	/*
	1. 구분자로 split, 구분자는 ", |, >, <, but, | < > >> <<는 살리거나 flag에 값을 넣어줌
	2. ""는 1차원 배열 구분자
	3. 파이프, 리다이렉션은 2차원 배열 구분자
	4. 파이프 리다이렉션으로 먼저 split, 리다이렉션은 구조체 플래그에 값 넣어줌.
	5. 쪼개진 문자열 만큼 구조체 할당하고
	6. 각 구조체에 쪼개진 문자열하나씩 ""로 split
	*/
	char	**cmd;

	cmd = divide_by_command(line, info);
	count_pipeline(cmd, info);
	make_command_array(cmd, info);
}

int	parse_line(char *line, t_info *info)
{
	char	*new_line;

	if (check_incorrect_line(line))
		return (ERROR);
	new_line = pre_processing(line, info);
	make_command(new_line, info);
	return (NORMAL);
}