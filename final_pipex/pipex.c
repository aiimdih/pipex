/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiimdih <aiimdih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:56:05 by aiimdih           #+#    #+#             */
/*   Updated: 2025/02/16 18:01:04 by aiimdih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	handle_here_doc(t_cmd *file, char **av)
{
	int		temp_fd;
	char	*line;

	file->limiter = av[2];
	file->here_doc = TRUE;
	ft_strlcpy(file->tmp, "tmp.txt", 8);
	while (access(file->tmp, F_OK) == 0)
		file->tmp[1] += 1;
	temp_fd = open(file->tmp, O_CREAT | O_RDWR | O_TRUNC, 0644);
	fail_check(temp_fd, file->tmp, file);
	line = "";
	while (write(1, "> ", 2), line != NULL)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, file->limiter, ft_strlen(file->limiter)) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	close(temp_fd);
}

void	handle_child(int children_sz, t_cmd *file, int *pipes, int command_size)
{
	if (children_sz == 0)
	{
		dup_and_close(pipes[1], STDOUT_FILENO);
		if (!file->here_doc)
			file->fd_in = open(file->input, O_RDONLY);
		else
			file->fd_in = open(file->tmp, O_RDONLY);
		dup_and_close(file->fd_in, STDIN_FILENO);
	}
	else
		dup_and_close(file->prev_in, STDIN_FILENO);
	if (children_sz == (command_size - 1))
	{
		file->fd_out = open(file->out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup_and_close(file->fd_out, STDOUT_FILENO);
	}
	else
		dup2(pipes[1], STDOUT_FILENO);
	close(pipes[1]);
	close(pipes[0]);
}

void	wait_childrens(int *pid, int children_sz)
{
	int	i;

	i = 0;
	while (i < children_sz)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

void	handle_fork(int command_size, char **commands, t_cmd *file)
{
	int	pipes[2];
	int	children_sz;

	children_sz = 0;
	file->pid = malloc(command_size * sizeof(int));
	while (children_sz < command_size)
	{
		pipe(pipes);
		file->pid[children_sz] = fork();
		if (file->pid[children_sz] == 0)
		{
			handle_child(children_sz, file, pipes, command_size);
			pre_excute_cmd(commands[children_sz], file);
		}
		close(pipes[1]);
		if (children_sz > 0)
			close(file->prev_in);
		file->prev_in = pipes[0];
		children_sz++;
	}
	close(pipes[0]);
	wait_childrens(file->pid, children_sz);
	free(file->pid);
}

int	main(int ac, char **av, char **env)
{
	char	**commands;
	t_cmd	*file;
	int		command_number;

	if (ac < 3)
		handle_errors("cmd under 3");
	commands = &av[2];
	file = &((t_cmd){0});
	file = malloc(sizeof(t_cmd));
	file->envp = env;
	file->here_doc = FALSE;
	command_number = ac - 3;
	if (check_here_doc(av[1]))
	{
		command_number = ac - 4;
		commands = &av[3];
		handle_here_doc(file, av);
	}
	else
		file->input = av[1];
	file->out = av[ac - 1];
	handle_fork(command_number, commands, file);
	if (file->here_doc)
		unlink(file->tmp);
	free(file);
}
