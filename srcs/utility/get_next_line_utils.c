/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeonkki <hyeonkki@student.42.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 16:20:48 by hyeonkki          #+#    #+#             */
/*   Updated: 2021/05/24 16:20:49 by hyeonkki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_bzero(void *ptr, size_t size)
{
	char	*s;
	size_t	i;

	i = 0;
	s = ptr;
	while (i < size)
	{
		s[i] = 0;
		i++;
	}
}

int		gnl_strmove(int fd, char **backup, char *src)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)malloc(ft_strlen(src) + 1);
	if (str == 0)
		return (0);
	while (src[i])
	{
		str[i] = src[i];
		i++;
	}
	str[i] = 0;
	free(backup[fd]);
	backup[fd] = str;
	return (1);
}

char	*ft_strchr(char *str, int c)
{
	int	i;

	i = -1;
	if (str == 0)
		return (0);
	while (str[++i])
		if (str[i] == c)
			return (str + i);
	if (str[i] == 0 && c == 0)
		return (str + i);
	return (0);
}

char	*gnl_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (str == 0)
		return (0);
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = 0;
	free((char *)s1);
	return (str);
}
