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

typedef struct d_cmd
{
	int 	input ;
	int		out ;
	char	**envp;
	char	*limiter;
	int		here_doc ;
	int     prev_in;
} t_cmd;

void dup_and_close(int fd, int fd_target)
{
	dup2(fd, fd_target);
	close(fd);
}

void handle_erros(char *Error_msg)
{
	perror(Error_msg);
	exit(1);
}

char *execute_command(char *cmds, char *path, char **env)
{
        char **cmd;
        cmd = ft_split(cmds, ' ');
        if (execve(path, cmd, env) == -1)
                printf("error");
		while(*cmd)
			free(*cmd++);
        return (NULL);
}

char *get_env(char **env, char *cmd)
{
        int i;
        char **all_paths;
        char *find_right_path;
        char *full_path;
        char **cmds;

        i = 0;
        while (ft_strnstr(env[i], "PATH=", 5) == 0)
                i++;
        all_paths = ft_split(env[i] + 5, ':');
        i = 0;
        while (all_paths[i++])
        {
                find_right_path = ft_strjoin(all_paths[i], "/");
                cmds = ft_split(cmd, ' ');
                full_path = ft_strjoin(find_right_path, cmds[0]);
				while (*cmds)
					free(*cmds++);
                if (access(full_path, F_OK) == 0)
                {
                        while (*all_paths)
                                free(*all_paths++);
                        free(find_right_path);
                        return (full_path);
                }
        }
        return 0;
}

void pre_excute_cmd(char *cmd, char **env)
{
	char *path;
	path = get_env(env, cmd);
    execute_command(cmd, path, env);
	free(path);
}

void handle_here_doc(t_cmd *file)
{
	int temp_fd;
	char *tmp = "tmp.txt";
 	char *line;

	if ((temp_fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644)) == -1)
		handle_erros("Error opening file1");
	write (1,">", 1);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strncmp(line, file->limiter, ft_strlen(file->limiter)) == 0)
		{
			free(line);
			break;
		}
		write (1,">", 1);
		write(temp_fd, line, ft_strlen(line));
		line = get_next_line(STDIN_FILENO);
	}
	close(temp_fd);
	if ((temp_fd = open(tmp, O_RDONLY)) == -1 )
		handle_erros("Error opening file1");
	dup_and_close(temp_fd , STDIN_FILENO);
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
	if (children_sz == command_size - 1)
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
	pid_t *pid;
	int pipes[2];
	int children_sz;
			
	children_sz = 0;
	pid = malloc(command_size * sizeof(int));
	while (children_sz < command_size)
	{
		pipe(pipes);
		pid[children_sz] = fork();
		if (pid[children_sz] == 0)
		{
			handle_child(children_sz, file, pipes, command_size);
			pre_excute_cmd(commands[children_sz],file->envp);
		}
		close(pipes[1]);
		file->prev_in = pipes[0];
		children_sz++;
	}
	wait_childrens(pid, children_sz);
	free(pid);
}

int main(int ac, char **av, char **env)
{
	int children_sz;
	char **commands;
	t_cmd *file;
	int command_number;

	if (ac < 3)
		handle_erros("cmd under 3");
	commands = &av[2];
	command_number = ac - 3;
	file = malloc(sizeof(t_cmd));
	file->envp = env;
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		command_number = ac - 4;
		commands = &av[3];
		file->limiter = av[2];
		file->here_doc = TRUE;
	}
	else if ((file->input = open(av[1], O_RDONLY)) == -1)
			handle_erros("Error opening file1");
	if ((file->out = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
		handle_erros("Error opening file1");
	handle_fork(command_number, commands, file);
	free(file);
}
