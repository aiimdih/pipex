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

int check_here_doc(char *file_name)
{
	if (ft_strncmp(file_name, "here_doc", 8) == 0)
		return TRUE;
	return FALSE;
}

void open_files(t_cmd *file)
{
	file->fd_out = open(file->out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file->fd_out == -1)
	{
		free(file);
		handle_errors(file->out);
	}
	if (!file->here_doc)
	{
		file->fd_in = open(file->input, O_RDONLY);
		if (file->fd_in == -1)
		{
			free(file);
			handle_errors(file->input);
		}
	}
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

void	put_errors(char *cmd)
{
	while (*cmd)
	{
		write(2, cmd, 1);
		cmd++;
	}
}

void	handle_errors(char *cmd)
{
	put_errors("pipex :  ");
	put_errors(cmd);
	put_errors(" ");
	perror("");
	exit(1);
}
