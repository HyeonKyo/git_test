/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 21:41:22 by kyunkim           #+#    #+#             */
/*   Updated: 2021/05/10 21:41:23 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_set(char c, char const *set)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(set);
	while (i < len)
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

int	get_first(char const *s1, char const *set)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s1[i] && check_set(s1[i], set))
	{
		count++;
		i++;
	}
	return (count);
}

int	get_last(char const *s1, char const *set, int first)
{
	int	i;
	int	len;
	int	count;

	i = 0;
	count = 0;
	len = ft_strlen(s1);
	while (len - i > first && check_set(s1[len - 1 - i], set))
	{
		count++;
		i++;
	}
	return (count);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s1_tmp;
	int		first;
	int		last;
	int		i;

	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	i = 0;
	first = get_first(s1, set);
	last = ft_strlen(s1) - get_last(s1, set, first);
	s1_tmp = (char *)malloc(sizeof(char) * (last - first + 1));
	if (!s1_tmp)
		return (NULL);
	while (last > first)
	{
		s1_tmp[i] = s1[first];
		i++;
		first++;
	}
	s1_tmp[i] = 0;
	return (s1_tmp);
}
