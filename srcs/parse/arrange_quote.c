#include "minishell.h"

static char	**divide_by_separator(char *line, int start_idx, int sep_idx)
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

static char	*make_buf(void)
{
	char	*buf;

	buf = (char *)malloc(sizeof(char) * BUF_SIZE);//나중에 디파인 상수로 변경
	merror(buf);
	return (buf);
}

static char	*make_arrange_string(char *mid, char **divide, int *start_idx)
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

char	*arrange_quote(char *line, int *start_idx, int sep_idx, t_info *info)
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