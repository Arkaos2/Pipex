#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include "libft/libft.h"

typedef struct s_exec
{
	int		fd_in;
	int		fd_out;
	char	*cmd_path;
	char	**cmd_args;
}	t_exec;

void	run_pipe(int fd_in, int fd_out, char **argv, char **envp);
void	run_first_cmd(int fd_in, int pipe_write, char **argv, char **envp);
void	run_second_cmd(int fd_out, int pipe_read, char **argv, char **envp);

char	*try_paths_for_cmd(char **paths, char **cmd);
char	**cmd_split(char **argv, int i);
char	**get_path(char **envp);
char	*build_and_check(char **argv, int cmd_index, char **envp);

void	fork_cmd_child(t_exec *cmd, char **envp);
void	exec_child(t_exec *cmd, char **envp);

#endif
