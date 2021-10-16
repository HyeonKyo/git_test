#include "minishell.h"

typedef struct s_env
{
	char	*key;
	char	*value;
	int		env_flag;
	t_env	*next;
	t_env	*prev;
}			t_env;

t_env	*create_env_node(void)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	merror(new);
	ft_memset(new, 0, sizeof(t_env));
	return (new);
}

t_env	*make_new_env_list(char **envp)
{
	int		i;
	char	**env_str;
	t_env	*env_list;
	t_env	*cur;
	t_env	*tmp;

	env_list = create_env_node();
	cur = env_list;
	i = -1;
	while (envp[++i])
	{
		if (i == 0)
			env_list = cur;
		else
		{
			tmp = create_env_node();
			tmp->prev = cur;
			cur->next = tmp;
			cur = cur->next;
		}
		env_str = env_split(envp[i]);
		if (env_str == NULL)
			continue ;
		cur->key = env_str[KEY];
		cur->value = env_str[VALUE];
		cur->env_flag = TRUE;
	}
	return (env_list);
}
