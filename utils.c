#include "libft.h"
//

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
void free_split_array(char **array) {
    if (!array) return; // Check if the array is NULL

    for (int i = 0; array[i]; i++) {
        free(array[i]); // Free the individual string
    }

    free(array);
}

