#include "minishell.h"


static void	*delete_mem(char **p, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free((*p + i));
		i++;
	}
	return (NULL);
}

static int	getarray(char const *str, char charset)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == charset && str[i])
			i++;
		if (str[i] != charset && str[i])
		{
			count++;
			while (str[i] != charset && str[i])
				i++;
		}
	}
	return (count);
}

static char	**allocarry(char const *str, char **p, char charset, int count)
{
	int				t;
	int				i;
	int				len;

	i = 0;
	t = 0;
	while (t < count)
	{
		len = 0;
		while (str[i] == charset && str[i])
			i++;
		if (str[i] != charset && str[i])
		{
			while (str[i] != charset && str[i])
			{
				i++;
				len++;
			}
			*(p + t) = (char *)malloc(sizeof(char) * (len + 1));
			if (!(*(p + t)))
				return (delete_mem(p, count));
			*(p[t++] + len) = 1;
		}
	}
	return (p);
}

static void	input(char const *str, char **p, char charset, int count)
{
	int				i;
	int				t;

	t = 0;
	while (t < count)
	{
		i = 0;
		while (*(p[t] + i) != 1)
		{
			if (*str != charset)
			{
				*(p[t] + i) = *str;
				i++;
			}
			str++;
		}
		*(p[t++] + i) = 0;
	}
}

char	**ft_split(char const *str, char charset)
{
	char			**p;
	int				count;

	if (!str)
		return (NULL);
	count = getarray(str, charset);
	p = (char **)malloc(sizeof(char *) * (count + 1));
	if (!p)
		return (NULL);
	p[count] = 0;
	if (count == 0)
		return (p);
	if (!(allocarry(str, p, charset, count)))
		return (NULL);
	input(str, p, charset, count);
	return (p);
}


int main()
{
	char *str = "asd basd";
	char **qq;
	char t[1];
	int i = 0;

	t[0] = '\0';
	qq = ft_split(str, ' ');
	i = 0;
	while (i < 3)
		printf("%s\n", qq[i++]);
	printf("size : %lu\n", sizeof(qq));
	return (0);
}
