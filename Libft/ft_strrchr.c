/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 18:59:33 by kyunkim           #+#    #+#             */
/*   Updated: 2021/05/10 18:59:34 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t	i;
	size_t	len;
	char	*s;

	i = 0;
	s = (char *)str;
	len = ft_strlen(s);
	while (i <= len)
	{
		if (s[len - i] == (char)c)
			return (s + (len - i));
		i++;
	}
	return (NULL);
}
