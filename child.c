/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saibelab <saibelab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:30:00 by saibelab          #+#    #+#             */
/*   Updated: 2025/09/15 18:30:00 by saibelab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_child(t_exec *cmd, char **envp)
{
	if (dup2(cmd->fd_in, STDIN_FILENO) == -1
		|| dup2(cmd->fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(cmd->fd_in);
	close(cmd->fd_out);
	if (!cmd->cmd_path || !cmd->cmd_args || !cmd->cmd_args[0])
		exit(127);
	if (!cmd->cmd_args[0] || cmd->cmd_args[0][0] == '\0')
	{
		ft_putstr_fd("Invalid command\n", 2);
		exit(1);
	}
	execve(cmd->cmd_path, cmd->cmd_args, envp);
	perror(cmd->cmd_args[0]);
	exit(127);
}

void	fork_cmd_child(t_exec *cmd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		exec_child(cmd, envp);
}

int	prepare_cmd(t_exec *cmd, char **envp, char ***old_args_ptr, char **old_path)
{
	char	*cmd_str;
	char	**args_split;

	cmd_str = cmd->cmd_args[cmd->current];
	args_split = ft_split(cmd_str, ' ');
	if (!args_split)
		return (0);
	if (!args_split[0] || args_split[0][0] == '\0')
	{
		ft_putstr_fd("Invalid Command\n", 2);
		free_split(args_split);
		return (0);
	}
	*old_args_ptr = cmd->cmd_args;
	*old_path = cmd->cmd_path;
	cmd->cmd_args = args_split;
	cmd->cmd_path = build_and_check(args_split, 0, envp, cmd->infile);
	if (!cmd->cmd_path)
	{
		free_split(args_split);
		cmd->cmd_args = *old_args_ptr;
		cmd->cmd_path = *old_path;
		return (0);
	}
	return (1);
}

void	run_single_cmd(t_exec *cmd, int fd_in, int fd_out, char **envp)
{
	char	*old_path;
	char	**old_args;

	cmd->fd_in = fd_in;
	cmd->fd_out = fd_out;
	if (!prepare_cmd(cmd, envp, &old_args, &old_path))
	{
		handle_empty_cmd(fd_in, fd_out);
		return ;
	}
	fork_cmd_child(cmd, envp);
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	free_split(cmd->cmd_args);
	cmd->cmd_args = old_args;
	if (cmd->cmd_path != old_path)
		free(cmd->cmd_path);
	cmd->cmd_path = old_path;
}

void	launch_commands(t_exec *cmd, int **pipes, int fd_out, char **envp)
{
	int	i;
	int	cur_in;
	int	cur_out;

	cur_in = cmd->fd_in;
	i = 0;
	while (i < cmd->nb_cmd)
	{
		if (i < cmd->nb_cmd - 1)
			cur_out = pipes[i][1];
		else
			cur_out = fd_out;
		if (i > 0)
			cur_in = pipes[i - 1][0];
		cmd->current = i;
		cmd->fd_in = cur_in;
		cmd->fd_out = cur_out;
		run_single_cmd(cmd, cur_in, cur_out, envp);
		if (i < cmd->nb_cmd - 1)
			cur_in = pipes[i][0];
		i++;
	}
}
