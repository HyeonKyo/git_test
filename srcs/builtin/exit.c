#include "minishell.h"

int	is_digit_string(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == 0)
		return (TRUE);
	return (FALSE);
}

void	execute_exit(char **cmd)
{
	int		i;
	char	*first_argv;
	t_exit	tmp;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd[1] == NULL)
		exit(0);
	if (is_digit_string(cmd[1]))
	{
		if (cmd[2] == NULL)
		{
			tmp.num = ft_atoi(cmd[1]);
			//g_exit_code = tmp.c[3];
			exit(0);//exit안의 값 exitcode로 변경해주어야함.
		}
		error_message(cmd[0], "too many arguments");
		return ;
	}
	else
	{
		error_message(cmd[1], "numeric argument required");
		exit(255);
	}
}
/*
exit명령어
1. 기본 : exit + (숫자)
2. exitcode에 숫자값의 뒤의 8비트값을 넣어주고(숫자 인자 없으면 0) exit출력 후 쉘을 종료시킴
3. exit + 숫자 여러개
	->exit 출력, too many arg에러메시지 출력, 종료 X
4. exit + 문자 or 문자 여러개
	->exit 출력, 숫자요구 에러메시지 출력, exitcode=255, 종료 O
5. exit + 숫자 + 문자
	-> 3과 동일
6. exit + 문자 + 숫자
	-> 4와 동일
*/
/*
exit출력
1. cmd[1] == NULL -> 0넣고 종료
2. is_digit_string(cmd[1]) and cmd[2] == NULL -> 숫자 넣고 종료
3. is_digit_string(cmd[1]) and cmd[2] != NULL -> too many출력 후 종료 X
4. not is_digit_string(cmd[1]) -> exitcode=255 need 숫자 출력 후 종료 O
*/