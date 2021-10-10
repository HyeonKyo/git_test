#include "minishell.h"

char	*make_cursor_string(void)
{
	int		idx;
	char	*cur_path;
	char	*last_path;
	char	*cursor;
	char	**tmp;

	cur_path = getcwd(NULL, 0);//상대경로(마지막 슬래쉬)파싱 함수 만들기
	merror(cur_path);
	tmp = ft_split(cur_path, '/');
	merror(tmp);
	free(cur_path);
	idx = 0;
	while (tmp[idx] != NULL)
		idx++;
	last_path = ft_strdup((char *)tmp[--idx]);
	free_env_list(tmp);
	cur_path = ft_strjoin("Minishell ", last_path);
	free(last_path);
	cursor = ft_strjoin(cur_path, ">");
	free(cur_path);
	return (cursor);
	//함수 정리하기!
}
