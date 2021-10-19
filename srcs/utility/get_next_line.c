/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeonkki <hyeonkki@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 16:19:54 by hyeonkki          #+#    #+#             */
/*   Updated: 2021/05/24 16:33:53 by hyeonkki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>//지우기
extern	int g_exit_code;

void	gnl_strcpy(char *dest, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (src == 0)
	{
		dest[i] = 0;
		return ;
	}
	while (i + 1 < size && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	if (size != 0)
		dest[i] = 0;
}

int		clean(int fd, char *buf, char **backup, int n)
{
	if (buf)
	{
		free(buf);
		buf = 0;
	}
	if (fd >= 0 && fd <= OPEN_MAX)
	{
		if (backup[fd])
		{
			free(backup[fd]);
			backup[fd] = 0;
		}
	}
	return (n);
}

int		gnl_get_one_line(int fd, char **line, char **backup, char *buf)
{
	char	*tmp;
	size_t	len;

	tmp = ft_strchr(backup[fd], '\n');
	if (tmp == 0)
		tmp = ft_strchr(backup[fd], '\0');
	len = tmp - backup[fd] + 1;
	*line = (char *)malloc(len * sizeof(char));
	if (*line == 0)
		return (clean(fd, buf, backup, -1));
	gnl_strcpy(*line, backup[fd], len);
	if (backup[fd] && backup[fd][0])
	{
		if (tmp[0] == 0)
			return (clean(fd, buf, backup, 0));
		else
		{
			if (!gnl_strmove(fd, backup, tmp + 1))
				return (clean(fd, buf, backup, -1));
			free(buf);
			return (1);
		}
	}
	else
		return (clean(fd, buf, backup, 0));
}

int		get_next_line(int fd, char **line)
{
	ssize_t		n_bytes;
	char		*buf;
	static char	*backup[OPEN_MAX + 1];

	buf = 0;
	if (BUFFER_SIZE < 1 || !(buf = (char *)malloc(BUFFER_SIZE + 1))
	|| fd < 0 || fd > OPEN_MAX || !line)
		return (clean(fd, buf, backup, -1));
	ft_bzero(buf, BUFFER_SIZE + 1);
	while ((n_bytes = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		backup[fd] = gnl_strjoin(backup[fd], buf);
		if (!backup[fd])
			return (clean(fd, buf, backup, -1));
		if (ft_strchr(backup[fd], '\n'))
			return (gnl_get_one_line(fd, line, backup, buf));
		ft_bzero(buf, BUFFER_SIZE + 1);
	}
	if (n_bytes == 0)
	{
		printf("@@@flag1@@@\n");
		return (gnl_get_one_line(fd, line, backup, buf));
	}
	else
		return (clean(fd, buf, backup, -1));
}
