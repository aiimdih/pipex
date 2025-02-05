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
	char *line;
	int temp_fd;
	char *tmp = "tmp.txt";
			
	children_sz = 0;
	pid = malloc(command_size * sizeof(int));
	while (children_sz < command_size)
	{
		pipe(pipes);
		pid[children_sz] = fork();
		if (pid[children_sz] == 0)
		{
			if (children_sz == 0) {
				if (!file->here_doc)
				{
					dup2(file->input, STDIN_FILENO);
					close(file->input);
				}
				else
				{
					if ((temp_fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644)) == -1)
					{
						printf("Error opening file1");
						exit(1);
					}
				//	close(file->input);
					write (1,">", 1);
					line = get_next_line(STDIN_FILENO);
					//printf("%s\n", line);
					while (line != NULL)
					{
						if (ft_strncmp(line, file->limiter, ft_strlen(file->limiter)) == 0)
						{
							free(line);
							break;
						}
						write (1,">", 1);
						write(temp_fd, line, ft_strlen(line));
					//	fprintf(stderr,"limiter --> %s\n", file->limiter);
						line = get_next_line(STDIN_FILENO);
					}
					dup2(temp_fd , STDIN_FILENO);
					close(temp_fd);
				//	fprintf(stderr,"limiter --> %s\n", file->limiter);
				}
			} else {
				dup2(prev_in, STDIN_FILENO);
				close(prev_in);
			}
			if (children_sz == command_size - 1)
			{
				dup2(file->out, STDOUT_FILENO);
				close(file->out);
			} else {
				dup2(pipes[1], STDOUT_FILENO);
			}
			close(pipes[1]);
			close(pipes[0]);
			handle_child(commands[children_sz],file->envp);
		}
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

	commands = &av[2];
	file = malloc(sizeof(t_cmd));
	file->envp = env;
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		commands = &av[3];
		file->limiter = ft_strdup(av[2]);
		file->here_doc = TRUE;
		printf("in --> %d \n", file->input);
	}
	else	if ((file->input = open(av[1], O_RDONLY)) == -1)
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
