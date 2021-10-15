/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeonkyokim <hyeonkyokim@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 05:24:59 by kyunkim           #+#    #+#             */
/*   Updated: 2021/10/15 13:14:06 by hyeonkyokim      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*str;

	len = 0;
	i = 0;
	j = 0;
	if (s1 == 0 && s2 == 0)
		return (0);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == 0)
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (i < len)
		str[i++] = s2[j++];
	str[i] = 0;
	return (str);
}
