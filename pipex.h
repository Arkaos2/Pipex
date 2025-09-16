/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saibelab <saibelab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:00:00 by saibelab          #+#    #+#             */
/*   Updated: 2025/09/15 16:00:00 by saibelab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <string.h>
# include <errno.h>
# include "libft/libft.h"

typedef struct s_exec
{
	int		fd_in;
	int		fd_out;
	int		nb_cmd;
	int		current;
	char	*cmd_path;
	char	**cmd_args;
}	t_exec;

char	*build_and_check(char **argv, int cmd_index, char **envp);
char	**cmd_split(char **argv, int i);
void	free_cmd(t_exec *cmd);

void	fill_cmd_args(t_exec *cmd, char **argv, int start);
void	exec_child(t_exec *cmd, char **envp);
void	fork_cmd_child(t_exec *cmd, char **envp);
void	handle_empty_cmd(int fd_in, int fd_out);
void	run_single_cmd(t_exec *cmd, int fd_in, int fd_out, char **envp);
void	launch_commands(t_exec *cmd, int **pipes, int fd_out, char **envp);
void	run_pipes(t_exec *cmd, int fd_in, int fd_out, char **envp);
void free_cmd(t_exec *cmd);

int		**create_pipes(int nb_cmd);
void	free_pipes(int **pipes, int nb_cmd);

int		open_here_doc(char **argv, int argc, int *fd_in, int *fd_out);
int		open_infile_outfile(char **argv, int argc, int *fd_in, int *fd_out);
int		open_files(char **argv, int argc, int *fd_in, int *fd_out);

t_exec	*prepare_exec(char **argv, int argc);

#endif
