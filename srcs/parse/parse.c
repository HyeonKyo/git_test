#include "minishell.h"

/*
** =============================================================================
** parse_utils.c
** =============================================================================
*/

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
	else if (is_special(c))
		return (SPEC);
	else if (c == '$')
		return (DOLR);
	else if (c == '\0')
		return (END);
	else if (is_redirection(c))
		return (is_redirection(c));
	return (NORM);
}

int	is_separator(t_type type)
{
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


int	check_incorrect_line(char *line)
{
	int	i;
	int	squote_flag;
	int	dquote_flag;
	int	squote_cnt;
	int	dquote_cnt;

	squote_flag = FALSE;
	dquote_flag = FALSE;
	i = 0;
	squote_cnt = 0;
	dquote_cnt = 0;
	while (line[i])
	{
		if (check_type(line[i]) == SQUOTE && dquote_flag == FALSE)
		{
			squote_cnt++;
			squote_flag ^= TRUE;
		}
		else if (check_type(line[i]) == DQUOTE && squote_flag == FALSE)
		{
			dquote_cnt++;
			dquote_flag ^= TRUE;
		}
		if (!squote_flag && !dquote_flag &&
			(line[i] == ';' || line[i] == '\\'))
			return (TRUE);
		//리다이렉션 체크 추가
		//1. <>처럼 다른 게 연속으로 올 떄
		//2. >>> 처럼 3개 이상이 연속으로 올때
		i++;
	}
	if ((squote_cnt & ISODD) || (dquote_cnt & ISODD))
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
			return (idx);
		idx++;
	}
	return (FALSE);
}

char	**divide_by_separator(char *line, int sep_idx)
{
	char	**str;
	int		front_len;
	int		back_len;

	str = (char **)malloc(sizeof(char *) * 2);
	merror(str);
	front_len = sep_idx + 1;
	back_len = ft_strlen(line) - sep_idx + 1;
	str[0] = (char *)malloc(sizeof(char) * front_len);
	merror(str[0]);
	str[1] = (char *)malloc(sizeof(char) * back_len);
	merror(str[1]);
	ft_strlcpy(str[0], line, front_len);
	ft_strlcpy(str[1], line + sep_idx, back_len);
	free(line);
	return (str);
}

char	*make_buf(void)
{
	char	*buf;

	buf = (char *)malloc(sizeof(char) * 2147483647);//나중에 디파인 상수로 변경
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
		key_len++;
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
	*origin = ft_strjoin(new, rest_str);
	free(front_str);
	free(env_value);
	free(rest_str);
	free(new);
}

char	*make_new_string(char *buf, int buf_len)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (buf_len + 3));
	merror(str);
	str[0] = '\"';
	ft_strlcpy(str + 1, buf, buf_len + 1);
	str[buf_len + 1] = '\"';
	str[buf_len + 2] = '\0';
	free(buf);
	return (str);
}

char	*fillin_buf(char *buf, char *origin, int start_idx, t_info *info)
{
	int		i;
	int		j;
	t_type	type;

	i = start_idx;
	j = 0;
	while (origin[i])
	{
		type = check_type(origin[i]);
		if (type == DQUOTE || type == SQUOTE)
		{
			i++;
			if (type == SQUOTE)
			{
				while (check_type(origin[i]) != SQUOTE)
					buf[j++] = origin[i++];
			}
			else
			{
				while (check_type(origin[i]) != DQUOTE)
				{
					if (type == DOLR)
					{
						replace_env_value(&origin, &i, info);
						continue ;
					}
					buf[j++] = origin[i++];
				}
			}
			i++;
		}
		else
			buf[j++] = origin[i++];
	}
	return (make_new_string(buf, j));
}

char	*make_arrange_string(char *front, char *back, int *start_idx)
{
	char	*new;

	*start_idx = ft_strlen(front);
	new = ft_strjoin(front, back);
	merror(new);
	return (new);
}

char	*clear_quote(char *line, int *start_idx, int sep_idx, t_info *info)
{
	/*
	1. space인덱스를 구분자로 2개의 문자열로 쪼개기
	2. int_max만큼 버퍼 할당.
	3. 버퍼 채우기
	4. 채워진 버퍼 + 스페이스 + \0만큼 재할당후 문자열 만들어주고 버퍼 프리
	5. 재할당 문자열과 기존 문자열 join후 start인덱스는 스페이스 다음으로 변경
	*/
	char	*buf;
	char	*new;
	char	**divide;

	divide = divide_by_separator(line, sep_idx);
	buf = make_buf();
	new = fillin_buf(buf, divide[0], *start_idx, info);
	new = make_arrange_string(new, divide[1], start_idx);
	free_env_list(divide);
	return (new);
}

/*
** =============================================================================
** parse_utils.c
** =============================================================================
*/

char	*remove_space(char *line, int start_idx)
{
	int	i;
	int	space_len;
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

	type = check_type(line[*i]);
	while (is_separator(type))
	{
		if (type == SPCE || type == END)
			return ;
		*i++;
		type = check_type(line[*i]);
	}
}

char	*skip_space(char *line, int *start_idx)
{
	int		i;
	char	*new;
	t_type	type;

	if (line[*start_idx])
		*start_idx++;
	i = *start_idx;
	new = ft_strdup(line);
	merror(new);
	free(line);
	type = check_type(line[i]);
	while (is_separator(type))
	{
		new = remove_space(new, i);
		skip_separator_not_space(new, &i);
	}
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
		if (new[sep_idx] == '\0')
			end_flag = TRUE;
		new = clear_quote(new, &start_idx, sep_idx, info);
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
	while (sep_idx)
	{
		sep_cnt++;
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
	info->n_pipeline = info->n_cmd;//나중에 개수 조절해줌.
	cmd = (char **)malloc(sizeof(char *) * (info->n_cmd + 1));
	merror(cmd);
	ft_memset(cmd, 0, sizeof(char *) * (info->n_cmd + 1));
	i = 0;
	pre_idx = 0;
	while (i < info->n_cmd)
	{
		cur_idx = find_separator(line, pre_idx);
		if (cur_idx == FALSE)
			cur_idx = (int)ft_strlen(line);
		cmd[i] = (char *)malloc(sizeof(char) * (cur_idx - pre_idx + 1));
		merror(cmd[i]);
		ft_strlcpy(cmd[i], line + pre_idx, cur_idx - pre_idx + 1);
		pre_idx = cur_idx;
		if (!is_redirection(line[pre_idx]))
			pre_idx++;
	}
	free(line);
	return (cmd);
}

int	check_redirection(t_cmd *cmds)
{
	int		i;
	char	*tmp;

	i = 0;
	while (is_redirection(cmds->cmd[0][i]))
		i++;
	if (i == 0)
		return (FALSE);
	else if (i == 1)
	{
		cmds->redi = check_type(cmds->cmd[0][1]);
		tmp = ft_strdup(cmds->cmd[0] + 1);
		free(cmds->cmd[0]);
		cmds->cmd[0] = tmp;
	}
	else
	{
		// <>는 예외처리하는 로직
		if (check_type(cmds->cmd[0][1]) == RRDI)
			cmds->redi = DRRDI;
		else
			cmds->redi = DLRDI;
		tmp = ft_strdup(cmds->cmd[0] + 2);
		free(cmds->cmd[0]);
		cmds->cmd[0] = tmp;
	}
	return (TRUE);
}

void	make_command_array(char **cmd, t_info *info)
{
	int		i;
	t_cmd	*cmds;

	cmds = (t_cmd *)malloc(sizeof(t_cmd) * info->n_cmd);
	merror(cmds);
	ft_memset(cmds, 0, sizeof(t_cmd) * info->n_cmd);
	i = 0;
	while (i < info->n_cmd)
	{
		cmds[i].cmd = ft_split(cmd[i], '\"');
		merror(cmds[i].cmd);
		if (check_redirection(&(cmds[i])))
			info->n_pipeline--;
		i++;
	}
	info->cmds = cmds;
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
	make_command_array(cmd, info);
}

int	parse_line(char *line, t_info *info)
{
	char	*cmd;

	if (check_incorrect_line(line))
		return (ERROR);
	cmd = pre_processing(line, info);
	make_command(cmd, info);
	return (NORMAL);
}
