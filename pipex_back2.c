/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiimdih <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:56:05 by aiimdih           #+#    #+#             */
/*   Updated: 2025/01/30 21:10:37 by aiimdih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

typedef struct d_cmd
{
	int 	input ;
	int		out ;
	char	**envp;
} t_cmd;

char *execute_command(char *cmds, char *path, char **env)
{
        char **cmd;
        cmd = ft_split(cmds, ' ');
        printf("%s\n", path);
        if (execve(path, cmd, env) == -1)
                printf("error");
        return (NULL);
}

char *get_env(char **env, char *cmd)
{
        int i;
        int j;
        char **all_paths;
        char *find_right_path;
        char *full_path;
        char **cmds;
        i = 0;
        while (ft_strnstr(env[i], "PATH=", 5) == 0)
                i++;
        all_paths = ft_split(env[i] + 5, ':');
        i = 0;
        while (all_paths[i])
        {
                find_right_path = ft_strjoin(all_paths[i], "/");
                cmds = ft_split(cmd, ' ');
                full_path = ft_strjoin(find_right_path, cmds[0]);
                if (access(full_path, F_OK) == 0)
                {
                        while (i >= 0)
                        {
                                free(all_paths[i]);
                                i--;
                        }
                        free(find_right_path);
                        return (full_path);
                }
                i++;
        }
        return 0;
}

void handle_child(char *cmd, char **env)
{
	char *path;
	path = get_env(env, cmd);
    execute_command(cmd, path, env);
	free(path);
}

void handle_fork(int command_size, char ** commands,  t_cmd *file)
{
	pid_t *pid;
	int pipes[2];
	int prev_in;
	int children_sz;

	children_sz = 0;
	pid = malloc(command_size * sizeof(int));
	while (children_sz < command_size)
	{
		pipe(pipes);
		pid[children_sz] = fork();
		if (pid[children_sz] == 0)
		{
			if (children_sz == 0) {
				dup2(file->input, STDIN_FILENO);
				close(file->input);
			} else 
				dup2(prev_in, STDIN_FILENO);
			if (children_sz == command_size - 1)
			{
				dup2(file->out, STDOUT_FILENO);
				close(file->out);
			} else {
				dup2(pipes[1], STDOUT_FILENO);
			}
			close(pipes[1]);
			close(pipes[0]);
			close(prev_in);
			handle_child(commands[children_sz],file->envp);
		}
		close(prev_in);
		close(pipes[1]);
		prev_in = pipes[0];
		children_sz++;
	}
	for (int j = 0; j < children_sz; j++)
		waitpid(pid[j], NULL, 0);
	free(pid);
}

int main(int ac, char **av, char **env)
{
	int children_sz;
	char **commands;
	t_cmd *file;

	file = malloc(sizeof(t_cmd));
	file->envp = env;
	commands = &av[2];
	if ((file->input = open(av[1], O_RDONLY)) == -1)
	{
		printf("Error opening file1");
		exit(1);
	}
	if ((file->out = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
	{
		printf("Error opening file1");
		exit(1);
	}
	handle_fork((ac - 3), commands, file);
	free(file);
}
