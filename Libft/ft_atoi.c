/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyunkim <kyunkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 16:50:26 by kyunkim           #+#    #+#             */
/*   Updated: 2021/05/14 16:50:28 by kyunkim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	unsigned long long	num;
	int					count;

	num = 0;
	count = 0;
	while (*str == 32 || *str == 9 || *str == 10 || \
			*str == 11 || *str == 12 || *str == 13)
		str++;
	if (*str == 45 || *str == 43)
	{
		if (*(str++) == 45)
			count++;
	}
	while ('0' <= *str && *str <= '9')
	{
		num = num * 10 + ((*str) - '0');
		str++;
	}
	if ((count % 2 == 0) && num > 9223372036854775807)
		return (-1);
	if ((count % 2 == 1) && num > 9223372036854775807)
		return (0);
	if (count % 2 == 1)
		num *= -1;
	return ((int)num);
}
