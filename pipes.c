#include "pipex.h"

int	**create_pipes(int nb_cmd)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * (nb_cmd - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < nb_cmd - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (NULL);
		if (pipe(pipes[i]) == -1)
			return (NULL);
		i++;
	}
	return (pipes);
}

void	free_pipes(int **pipes, int nb_cmd)
{
	int	i;

	i = 0;
	while (i < nb_cmd - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	run_pipes(t_exec *cmd, int fd_in, int fd_out, char **envp)
{
	int	**pipes;
	int status;

	pipes = create_pipes(cmd->nb_cmd);
	if (!pipes)
		return ;
	cmd->fd_in = fd_in;
	launch_commands(cmd, pipes, fd_out, envp);
	while (wait(&status) > 0)
		;
	(void)status;
	free_pipes(pipes, cmd->nb_cmd);
}

void free_cmd(t_exec *cmd)
{
    if (!cmd)
        return;
    if (cmd->cmd_path)
        free(cmd->cmd_path);
    if (cmd->cmd_args)
        free(cmd->cmd_args);
    free(cmd);
}
