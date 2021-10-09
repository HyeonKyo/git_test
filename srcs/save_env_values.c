# include "minishell.h"

void	save_env_values(t_info *info, char **envp)
{
	int	i;

	info->env_list = (char **)malloc(sizeof(char *) * sizeof(envp));
	merror(info->env_list);
	while (envp[i] != NULL)
	{
		info->env_list[i] = ft_strdup(envp[i]);
		i++;
	}
	info->env_list[i] = NULL;
}