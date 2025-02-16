/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiimdih <aiimdih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:18:57 by aiimdih           #+#    #+#             */
/*   Updated: 2025/02/14 20:34:35 by aiimdih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	fail_check(int fd, char *str, t_cmd *file)
{
	if (fd == -1)
	{
		if (file->pid)
			free(file->pid);
		free(file);
		handle_errors(str);
	}
}

int	check_here_doc(char *file_name)
{
	if (ft_strncmp(file_name, "here_doc", 8) == 0)
		return (TRUE);
	return (FALSE);
}

void	free_split(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void	dup_and_close(int fd, int fd_target)
{
	dup2(fd, fd_target);
	close(fd);
}

void	handle_errors(char *cmd)
{
	write(2, "pipex :  ", 9);
	write(2, cmd, ft_strlen(cmd));
	write(2, " ", 1);
	perror("");
	exit(1);
}
