#ifndef MINISHELL_H
# define MINISHELL_H

/*
** =============================================================================
** Dependencies
** =============================================================================
*/

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"

/*
** =============================================================================
** Macro Declaration
** =============================================================================
*/

# define TRUE 1
# define FALSE 0

# define ERROR 1
# define NORMAL 0


/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/

typedef struct s_minishell
{
	char	**environment_path;
	char	*limiter;
	int		is_here_doc;
}			t_minishell;

/*
** =============================================================================
** Struct type Definition
** =============================================================================
*/


#endif
