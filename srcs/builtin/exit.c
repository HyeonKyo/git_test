#include "minishell.h"

extern int	g_exit_code;

static t_ll	save_number(const char *str, int *idx, int *digit_len)
{
	int			minus;
	long long	num;

	idx = 0;
	minus = 1;
	num = 0;
	while ((str[*idx] >= 9 && str[*idx] <= 13) || str[*idx] == ' ')
		(*idx)++;
	if (str[*idx] == '-' || str[*idx] == '+')
		if (str[(*idx)++] == '-')
			minus = -1;
	while (str[*idx] >= '0' && str[*idx] <= '9')
	{
		num = num * 10 + (str[(*idx)++] - '0');
		(*digit_len)++;
	}
	while ((str[*idx] >= 9 && str[*idx] <= 13) || str[*idx] == ' ')
		(*idx)++;
	return (num);
}

static int	long_num_len(t_ll num)
{
	int	len;

	len = 0;
	while (num)
	{
		len++;
		num /= 10;
	}
	return (len);
}

static int	ft_atolong(const char *str, t_ll *val)
{
	int			i;
	int			digit_len;

	i = 0;
	digit_len = 0;
	if (str == NULL)
		return (FALSE);
	*val = save_number(str, &i, &digit_len);
	if (str[i] != '\0')
		return (FALSE);
	if (digit_len > 19 || long_num_len(*val) != digit_len)//num의 길이가 digit_len과 다른경우 => 테스트 해보기
		return (FALSE);
	return (TRUE);
}

static int	is_digit_string(char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == 0 && i <= 19)
		return (TRUE);
	return (FALSE);
	/*
		++모든 문자가 숫자이지만 longlong범위를 벗어나는 경우 문자열로 인식해야 하므로
		여기서 long long 범위 체크를 해줌.
		long long max = 9223372036854775807
	*/
}

void	execute_exit(char **cmd)
{
	int		i;
	char	*first_argv;
	t_exit	tmp;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd[1] == NULL)
		exit(0);
	if (is_digit_string(cmd[1]) && ft_atolong(cmd[1], &(tmp.num)))
	{
		if (cmd[2] == NULL)
		{
			g_exit_code = tmp.c[3];//3번 바꾸기
			exit(g_exit_code);//exit안의 값 exitcode로 변경해주어야함.
		}
		error_message(cmd[0], NULL, "too many arguments");
		return ;
	}
	error_message("exit", cmd[1], "numeric argument required");
	exit(255);
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