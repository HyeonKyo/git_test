/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 19:45:44 by kyunkim           #+#    #+#             */
/*   Updated: 2021/03/07 18:39:13 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dst_length;
	size_t	src_length;

	i = 0;
	j = ft_strlen(dest);
	src_length = ft_strlen(src);
	dst_length = ft_strlen(dest);
	if (size == 0)
		return (src_length);
	if (size <= dst_length)
		return (size + src_length);
	while (src[i] && dst_length + i < size - 1)
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	if (dst_length > size)
		return (src_length + size);
	else
		return (src_length + dst_length);
}
