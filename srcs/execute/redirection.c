#include "minishell.h"

/*
** =============================================================================
** gnl
** =============================================================================
*/
char	*gnl_strjoin(char *s1, char const *s2)
{
	char		*ret;
	int			s1_len;
	int			s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return ((char *)s2);
	else if (!s2)
		return ((char *)s1);
	ret = (char *)malloc(s1_len + s2_len + 1);
	ft_strlcpy(ret, s1, s1_len + 1);
	ft_strlcat(ret, s2, s2_len + s1_len + 1);
	free(s1);
	return (ret);
}

int	gnl_check_line_feed(char *save)
{
	int	i;

	i = 0;
	if (!save)
		return (-1);
	while (save[i] != 0)
	{
		if (save[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int	gnl_check_len(char **line, char **save)
{
	char	*backup;
	size_t	bk_len;
	int		i;

	i = gnl_check_line_feed(*save);
	bk_len = ft_strlen(*save) - (i + 1);
	*(*save + i) = '\0';
	*line = ft_strdup(*save);
	if (bk_len == 0)
	{
		free(*save);
		*save = NULL;
	}
	else
	{
		backup = ft_strdup(*save + i + 1);
		free(*save);
		*save = backup;
	}
	return (1);
}

int	gnl_return(char **line, char **save)
{
	int		length;

	length = gnl_check_line_feed(*save);
	if (length >= 0)
		return (gnl_check_len(line, save));
	if (*save)
	{
		*line = *save;
		*save = NULL;
	}
	else
		*line = ft_strdup("");
	return (0);
}

int	get_next_line(int fd, char **line)
{
	int			read_size;
	static char	*save[MAX_FD];
	char		*buff;

	if (fd < 0 || fd > MAX_FD || !line || BUF_SIZE <= 0)
		return (-1);
	buff = (char *)malloc(sizeof(char) * BUF_SIZE + 1);
	while (gnl_check_line_feed(save[fd]) == -1)
	{
		read_size = read(fd, buff, BUF_SIZE);
		if (read_size <= 0)
			break ;
		buff[read_size] = '\0';
		if (save[fd] == NULL)
			save[fd] = ft_strdup(buff);
		else
			save[fd] = gnl_strjoin(save[fd], buff);
	}
	free(buff);
	if (read_size < 0)
		return (-1);
	return (gnl_return(line, &save[fd]));
}
/*=============================================================================*/



/*
** =============================================================================
** here_doc
** =============================================================================
*/

int	read_string_from_stdin(t_info *info, char *limiter)
{
	char	*str;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	while (1)
	{
		info->pipex.is_here_doc = 1;
		get_next_line(0, &str);//gnl 함수로 표준입력 받기
		if (strncmp(str, limiter, ft_strlen(limiter)) == 0)//사용자가 limiter 입력하면 break
			break ;
		ft_putstr_fd(str, pipe_fd[WRITE]);//받은 문자열을 파이프에 저장
		ft_putstr_fd("\n", pipe_fd[WRITE]);
		free(str);
	}
	info->pipex.is_here_doc = 0;
	close(pipe_fd[WRITE]);
	return (pipe_fd[READ]);//문자열이 저장된 파이프의 fd를 반환
}

int	here_doc(t_info *info, char *limiter, int fd[])
{
	fd[READ] = read_string_from_stdin(info, limiter);//파이프의 fd 리턴, 에러 시 -1 리턴
	if (fd[READ] == -1)
		return (ERROR);
	return (NORMAL);
}
/*=============================================================================*/



int	input_redirection(t_info *info, char *infile, int fd[])
{
	fd[READ] = open(infile, O_RDWR);
	if (fd[READ] == -1)
		return (ERROR);
	return (NORMAL);
}

int	output_redirection(t_info *info, char *outfile, int fd[])
{
	fd[WRITE] = open(outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd[WRITE] == -1)
		return (ERROR);
	return (NORMAL);
}

int	d_output_redirection(t_info *info, char *outfile, int fd[])
{
	fd[WRITE] = open(outfile, O_CREAT | O_RDWR | O_APPEND, 0644);//">>" 일 때는 open() 함수 옵션에 O_APPEND
	if (fd[WRITE] == -1)
		return (ERROR);
	return (NORMAL);
}

int	redirection(t_info *info, int fd[])
{
	t_lst	*cur;
	t_lst	*next;
	int		reval;
	char	**redi;

	reval = NORMAL;
	cur = info->cmd_lst[info->cmd_sequence].redi;
	while (cur != NULL)
	{
		redi = ft_split(cur->str, '\"');
		if (!ft_strncmp(redi[0], "<<", 2))
			reval = here_doc(info, redi[1], fd);
		else if (!ft_strncmp(redi[0], ">>", 2))
			reval = d_output_redirection(info, redi[1], fd);
		else if (redi[0][0] == '<')
			reval = input_redirection(info, redi[1], fd);
		else if (redi[0][0] == '>')
			reval = output_redirection(info, redi[1], fd);
		if (reval)
			return (ERROR);
		cur = cur->next;
	}
	return (NORMAL);
}