/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 16:00:49 by kyunkim           #+#    #+#             */
/*   Updated: 2021/05/10 16:03:19 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t len)
{
	unsigned char	c;
	unsigned char	*str;
	size_t			i;

	i = 0;
	str = (unsigned char *)ptr;
	c = (unsigned char)value;
	while (i < len)
		str[i++] = c;
	return (str);
}
