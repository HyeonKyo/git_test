/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 12:18:34 by kyunkim           #+#    #+#             */
/*   Updated: 2021/05/13 12:18:36 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dst_tmp;
	unsigned char	*src_tmp;

	dst_tmp = (unsigned char *)dst;
	src_tmp = (unsigned char *)src;
	i = -1;
	if (dst == src)
		return (dst);
	if (dst < src)
	{
		while (++i < n)
			dst_tmp[i] = src_tmp[i];
	}
	else
	{
		while (++i < n)
			dst_tmp[n - i - 1] = src_tmp[n - i - 1];
	}
	return (dst);
}
