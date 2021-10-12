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

t_type	is_quote(char c)
{
	if (c == '\"')
		return (DQUOTE);
	else if (c == '\'')
		return (SQUOTE);
	return (NORM);
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
		if (check_type(line[i]) == SQUOTE)
		{
			squote_cnt++;
			squote_flag ^= TRUE;
		}
		else if (check_type(line[i]) == DQUOTE)
		{
			dquote_cnt++;
			dquote_flag ^= TRUE;
		}
		if (!squote_flag && !dquote_flag
			&& (line[i] == ';' || line[i] == '\\'))
			return (TRUE);
		i++;
	}
	if ((squote_cnt & 1) || (dquote_cnt & 1))
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
		if (type == SQUOTE)
			squote_flag ^= TRUE;
		else if (type == DQUOTE)
			dquote_flag ^= TRUE;
		if (!squote_flag && !dquote_flag && (type == SPCE || type == PIPE
			|| type == END || is_redirection(line[idx])))
			return (idx);
		idx++;
	}
	return (FALSE);
}

char	**divide_by_separator(char *line, int start_idx, int sep_idx)
{
	char	**str;
	int		front_len;
	int		back_len;

	str = (char **)malloc(sizeof(char *) * 2);
	merror(str);
	front_len = sep_idx - start_idx + 1;
	back_len = ft_strlen(line) - sep_idx + 1;
	str[0] = (char *)malloc(sizeof(char) * front_len);
	merror(str[0]);
	str[1] = (char *)malloc(sizeof(char) * back_len);
	merror(str[1]);
	ft_strlcpy(str[0], line + start_idx, front_len);
	ft_strlcpy(str[1], line + sep_idx, back_len);
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
				while (check_type(origin[i]) != SQUOTE)
					buf[j++] = origin[i++];
				i++;
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

void	clear_string(char *line, char **divide)
{
	free(line);
	free(divide[0]);
	free(divide[1]);
	free(divide);
}

char	*clean_quote_before_space(char *line, int *start_idx, int sep_idx, t_info *info)
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

	divide = divide_by_separator(line, *start_idx, sep_idx);
	buf = make_buf();
	new = fillin_buf(buf, divide[0], info);
	new = make_arrange_string(new, divide[1], start_idx);
	clear_string(line, divide);//free
	return (new);
}

/*
** =============================================================================
** parse_utils.c
** =============================================================================
*/

void	skip_separator(char *line, int *start_idx)
{
	while (line[*start_idx] && find_separator(line, *start_idx))
		(*start_idx)++;
}

char	*pre_processing(char *line, t_info *info)
{
	int		start_idx;
	int		sep_idx;
	char	*new;

	start_idx = 0;
	sep_idx = 0;
	new = ft_strdup(line);
	sep_idx = find_separator(new, sep_idx);//space뿐만 아니라 파이프,리다이렉션도 구분자가 됨.
	while (sep_idx)
	{
		new = clean_quote_before_space(new, &start_idx, sep_idx, info);
		skip_separator(new, &start_idx);
		sep_idx = find_separator(new, start_idx);
	}
	return (new);
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
