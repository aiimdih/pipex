#include "libft.h"

void free_split(char **ptr)
{
	int i;
	
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}
void dup_and_close(int fd, int fd_target)
{
	dup2(fd, fd_target);
	close(fd);
}

void put_errors(char *cmd)
{
	while (*cmd)
	{
		write(2, cmd, 1);
		cmd++;
	}
}

void handle_errors(char *cmd)
{
	put_errors("pipex :  ");
	put_errors(cmd);
	put_errors(" ");
	perror("");
	exit(1);
}
