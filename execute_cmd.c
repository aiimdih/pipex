#include "libft.h"

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

char *search_path(char *cmd, char **all_paths, int i)
{
	char *full_path;
	char *find_right_path;
	char **cmds;

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
	handle_errors(cmd);
	return 0;
}
char *get_env(char **env, char *cmd)
{
        int i;
        char **all_paths;
        char **cmds;

        i = 0;
        while (ft_strnstr(env[i], "PATH=", 5) == 0)
                i++;
        all_paths = ft_split(env[i] + 5, ':');
        i = 0;
		if (ft_strnstr(cmd, "/", ft_strlen(cmd)) == 0)
		{
			return(search_path(cmd, all_paths, i));
		}
		else
		{
			if (access(cmd, F_OK) == 0)
				if (access(cmd, X_OK) == 0)
					return (cmd);
				else
					handle_errors(cmd);
			else 
				handle_errors(cmd);
			return 0;
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

