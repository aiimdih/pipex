/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiimdih <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:18:29 by aiimdih           #+#    #+#             */
/*   Updated: 2025/02/14 13:18:35 by aiimdih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*execute_command(char *cmds, char *path, char **env)
{
	char	**cmd;

	cmd = ft_split(cmds, ' ');
	if (execve(path, cmd, env) == -1)
		printf("error");
	while (*cmd)
		free(*cmd++);
	return (NULL);
}

char	*search_path(char *cmd, char **all_paths)
{
	char	*full_path;
	char	*find_right_path;
	char	**cmds;
	int		i;

	i = 0;
	cmds = ft_split(cmd, ' ');
	if (!cmds || !cmds[0])
	{
		handle_errors(cmd);
		return (NULL);
	}
	while (all_paths[i])
	{
		find_right_path = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(find_right_path, cmds[0]);
		free(find_right_path);
		if (access(full_path, F_OK) == 0)
			return (free_split(cmds), full_path);
		free(full_path);
		i++;
	}
	return (free_split(cmds), free_split(all_paths), NULL);
}

char	*get_env(char **env, char *cmd)
{
	int		i;
	char	**all_paths;
	char	**cmds;

	i = 0;
	if (ft_strnstr(cmd, "/", ft_strlen(cmd)) != 0)
	{
		if (access(cmd, F_OK) == 0)
			if (access(cmd, X_OK) == 0)
				return (cmd);
	}
	else
	{
		while (ft_strnstr(env[i], "PATH=", 5) == 0)
			i++;
		all_paths = ft_split(env[i] + 5, ':');
		return (search_path(cmd, all_paths));
	}
	return (NULL);
}

void	pre_excute_cmd(char *cmd, t_cmd *file)
{
	char	*path;

	path = get_env(file->envp, cmd);
	if (path == NULL)
	{
		free(file->pid);
		free(file);
		handle_errors(cmd);
	}
	execute_command(cmd, path, file->envp);
	free(path);
}
