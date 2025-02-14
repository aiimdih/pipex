/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiimdih <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:56:05 by aiimdih           #+#    #+#             */
/*   Updated: 2025/02/01 20:56:33 by aiimdih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line/get_next_line.h"

void handle_here_doc(t_cmd *file)
{
	int temp_fd;
	char tmp[] = "tmp.txt";
 	char *line;
	
	while (access(tmp, F_OK) == 0)
		tmp[1] += 1;
	if ((temp_fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644)) == -1)
		handle_errors(tmp);
	while (write (1,">", 1), line != NULL)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, file->limiter, ft_strlen(file->limiter)) == 0)
		{
			free(line);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
	}
	close(temp_fd);
	if ((temp_fd = open(tmp, O_RDONLY)) == -1 )
		handle_errors(tmp);
	dup_and_close(temp_fd , STDIN_FILENO);
	unlink(tmp);
}

void	handle_child(int children_sz, t_cmd *file, int *pipes, int command_size) 
{
	if (children_sz == 0)
	{
		if (!file->here_doc) 
			dup_and_close(file->input, STDIN_FILENO);
		else
			handle_here_doc(file);
	}
	else 
		dup_and_close(file->prev_in, STDIN_FILENO);
	if (children_sz == (command_size - 1))
		dup_and_close(file->out, STDOUT_FILENO);
	else 
		dup2(pipes[1], STDOUT_FILENO);
	close(pipes[1]);
	close(pipes[0]);
}

void wait_childrens(int *pid, int children_sz)
{
	int i;

	i = 0;
	while (i < children_sz)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

void handle_fork(int command_size, char ** commands,  t_cmd *file)
{				
	pid_t pid[command_size];
	int pipes[2];
	int children_sz;
			
	children_sz = 0;
//	pid = malloc(command_size * sizeof(int));
	while (children_sz < command_size)
	{
		pipe(pipes);
		pid[children_sz] = fork();
		if (pid[children_sz] == 0)
		{
			handle_child(children_sz, file, pipes, command_size);
			pre_excute_cmd(commands[children_sz],file);
		}
		close(pipes[1]);
		if (children_sz > 0)
			close(file->prev_in);
		file->prev_in = pipes[0];
		children_sz++;
	}
	close(pipes[0]);
	close(file->input);
	close(file->out);
	wait_childrens(pid, children_sz);
}

int main(int ac, char **av, char **env)
{
	int children_sz;
	char **commands;
	t_cmd *file;
	int command_number;

	if (ac < 3)
		handle_errors("cmd under 3");
	commands = &av[2];
	command_number = ac - 3;
	file = malloc(sizeof(t_cmd));
	file->envp = env;
	file->here_doc = FALSE;
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		command_number = ac - 4;
		commands = &av[3];
		file->limiter = av[2];
		file->here_doc = TRUE;
	}
	else if ((file->input = open(av[1], O_RDONLY)) == -1)
			handle_errors(av[1]);
	if ((file->out = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
		handle_errors(av[ac - 1]);
	handle_fork(command_number, commands, file);
	free(file);
}
