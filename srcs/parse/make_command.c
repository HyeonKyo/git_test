#include "minishell.h"

static int	count_command(char *line)
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

static char	**create_cmd(char *line, t_info *info)
{
	char	**cmd;

	info->n_cmd = count_command(line);
	cmd = (char **)malloc(sizeof(char *) * (info->n_cmd + 1));
	merror(cmd);
	ft_memset(cmd, 0, sizeof(char *) * (info->n_cmd + 1));
	return (cmd);
}

static char	**divide_by_command(char *line, t_info *info)
{
	int		i;
	int		pre_idx;
	int		cur_idx;
	char	**cmd;

	cmd = create_cmd(line, info);
	i = -1;
	pre_idx = 0;
	cur_idx = find_separator(line, pre_idx);
	while (++i < info->n_cmd)
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
	}
	free(line);
	return (cmd);
}

static void	count_pipeline(char **cmd, t_info *info)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
		i++;
	info->n_pipe = i - 1;
}

static void	make_command_array(char **cmd, t_info *info)
{
	int		i;
	int		j;
	char	**split_cmd;
	t_cmd	*cmd_lst;

	cmd_lst = (t_cmd *)malloc(sizeof(t_cmd) * info->n_cmd);
	merror(cmd_lst);
	ft_memset(cmd_lst, 0, sizeof(t_cmd) * info->n_cmd);
	i = -1;
	while (++i < info->n_cmd)
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
		{
			if (is_redirection(split_cmd[j][0]))
				link_node(split_cmd[j], &(cmd_lst[i].redi));
			else
				link_node(split_cmd[j], &(cmd_lst[i].text));
			j++;
		}
		free_double_string(split_cmd);
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