#include "pipex.h"

void	free_cmd(t_exec *cmd)
{
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->cmd_args)
		free_split(cmd->cmd_args);
}

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
		exit(127);  // commande invalide

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

void	run_first_cmd(int fd_in, int pipe_write, char **argv, char **envp)
{
	t_exec	cmd;

	cmd.fd_in = fd_in;
	cmd.fd_out = pipe_write;
	cmd.cmd_path = build_and_check(argv, 2, envp);
	cmd.cmd_args = cmd_split(argv, 2);

	if (!cmd.cmd_path || !cmd.cmd_args || !cmd.cmd_args[0])
	{
		free_cmd(&cmd);
		close(pipe_write);
		return ;
	}

	fork_cmd_child(&cmd, envp);
	close(pipe_write);
	free_cmd(&cmd);
}

void	run_second_cmd(int fd_out, int pipe_read, char **argv, char **envp)
{
	t_exec	cmd;

	cmd.fd_in = pipe_read;
	cmd.fd_out = fd_out;
	cmd.cmd_path = build_and_check(argv, 3, envp);
	cmd.cmd_args = cmd_split(argv, 3);

	if (!cmd.cmd_path || !cmd.cmd_args || !cmd.cmd_args[0])
	{
		free_cmd(&cmd);
		close(pipe_read);
		return ;
	}

	fork_cmd_child(&cmd, envp);
	close(pipe_read);
	free_cmd(&cmd);
}

void	run_pipe(int fd_in, int fd_out, char **argv, char **envp)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	run_first_cmd(fd_in, pipefd[1], argv, envp);
	run_second_cmd(fd_out, pipefd[0], argv, envp);

	close(pipefd[0]);
	close(pipefd[1]);

	wait(NULL);
	wait(NULL);
}
