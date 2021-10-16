#include "minishell.h"

char	**env_split(char *str)
{
	int		key_len;
	int		val_len;
	char	*sep;
	char	**env;

	env = (char **)malloc(sizeof(char *) * 3);
	merror(env);
	sep = ft_strchr(str, '=');
	if (sep == NULL)
		return (NULL);
	key_len = sep - str + 1;
	val_len = (int)ft_strlen(sep);
	env[KEY] = (char *)malloc(sizeof(char) * key_len);
	merror(env[KEY]);
	ft_strlcpy(env[KEY], str, key_len);
	env[VALUE] = ft_strdup(sep + 1);
	merror(env[VALUE]);
	env[2] = NULL;
	return (env);
}