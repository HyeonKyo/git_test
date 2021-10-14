#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	len = 0;
	i = 0;
	if (src == NULL)
		return (0);
	len = ft_strlen(src);
	while (i + 1 < size && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	if (size != 0)
		dest[i] = '\0';
	return (len);
}
