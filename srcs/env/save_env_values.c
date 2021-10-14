# include "minishell.h"

void	save_env_values(t_info *info, char **envp)
{
	int	i;
	int	len;

	len = 0;
	while (envp[len++])
		;
	info->env_list = (char **)malloc(sizeof(char *) * len);
	merror(info->env_list);
	i = 0;
	while (envp[i] != NULL)
	{
		info->env_list[i] = ft_strdup(envp[i]);
		i++;
	}
	info->env_list[i] = NULL;
}
