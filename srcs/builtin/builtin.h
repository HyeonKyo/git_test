#ifndef BUILTIN_H
# define BUILTIN_H

# define READ 0
# define WRITE 1

int		cd(char *path);
int		pwd(int *fd);
int		error(void);
void	merror(void *addr);

#endif
