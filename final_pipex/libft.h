/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiimdih <aiimdih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:31:01 by aiimdih           #+#    #+#             */
/*   Updated: 2025/02/16 18:07:45 by aiimdih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include "get_next_line/get_next_line.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <unistd.h>
# define TRUE 1
# define FALSE 0

typedef struct d_cmd
{
	char	**envp;
	char	*limiter;
	char	*input;
	char	*out;
	char	tmp[8];
	int		fd_in;
	int		fd_out;
	int		here_doc;
	int		prev_in;
	pid_t	*pid;
}			t_cmd;
int			check_here_doc(char *file_name);
void		fail_check(int fd, char *str, t_cmd *file);
char		**ft_split(const char *s, char c);
char		*search_path(char *cmd, char **all_paths);
void		free_split(char **ptr);
char		*ft_strchr(const char *str, size_t *new_pos, int search_str);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strnstr(const char *str, const char *to_find, size_t len);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlcat(char *dest, char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlen(const char *str);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
void		dup_and_close(int fd, int fd_target);
void		handle_errors(char *Error_msg);
char		*execute_command(char *cmds, char *path, char **env);
char		*get_env(char **env, char *cmd);
void		pre_excute_cmd(char *cmd, t_cmd *file);
#endif
