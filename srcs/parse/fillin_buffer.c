#include "minishell.h"

static char	*parse_env_after_dollar(char **origin, int *i, t_info *info)
{
	t_type	type;
	int		pre_idx;
	char	*env_key;
	char	*env_value;

	pre_idx = *i;
	type = check_type((*origin)[*i]);
	while (type == NORM)
		type = check_type((*origin)[++(*i)]);
	env_key = (char *)malloc(sizeof(char) * (*i - pre_idx + 1));
	merror(env_key);
	ft_strlcpy(env_key, (*origin + *i), (*i - pre_idx + 1));//"aa$"같은 경우 테스트 해보기
	env_value = get_env_value(env_key, info);
	free(env_key);
	return (env_value);
}

static void	replace_env_value(char **origin, int *i, t_info *info)
{
	int		env_idx;
	char	*env_value;
	char	*front_str;
	char	*rest_str;
	char	*tmp;

	*i += 1;
	front_str = (char *)malloc(sizeof(char) * *i);
	merror(front_str);
	ft_strlcpy(front_str, *origin, *i);
	env_value = parse_env_after_dollar(origin, i, info);
	rest_str = ft_strdup(*origin + *i);
	merror(rest_str);
	*i = ft_strlen(front_str);
	tmp = ft_strjoin(front_str, env_value);
	free(*origin);
	*origin = ft_strjoin(tmp, rest_str);
	free(front_str);
	free(env_value);
	free(rest_str);
	free(tmp);
}

static char	choose_quote_type(char *buf, int buf_len)
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
	t_quote	data;

	i = 0;
	j = 0;
	init_quote_data(&data);
	while (origin[i])
	{
		if ((origin[i] == '\'' && !data.dquote_flag)
			|| (origin[i] == '\"' && !data.squote_flag))
		{
			if (origin[i] == '\'')
				data.squote_flag ^= TRUE;
			else
				data.dquote_flag ^= TRUE;
			i++;
		}
		else if (data.dquote_flag == TRUE && origin[i] == '$')
			replace_env_value(&origin, &i, info);
		else
			buf[j++] = origin[i++];
	}
	return (make_new_string(buf, j));
}