# include "minishell.h"
#include "builtin.h"

int	save_env_values(t_minishell	*minishell, char **envp)
{
	int	i;

	minishell->environment = (char **)malloc(sizeof(char *) * sizeof(envp));
	merror(minishell->environment);
	while (envp[i] != NULL)
	{
		minishell->environment[i] = ft_strdup(envp[i]);
		i++;
	}
	minishell->environment[i] = NULL;
}