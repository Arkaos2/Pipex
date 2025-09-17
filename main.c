/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saibelab <saibelab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:38:33 by saibelab          #+#    #+#             */
/*   Updated: 2025/09/17 16:38:33 by saibelab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_infile_outfile(char **argv, int argc, int *fd_in, int *fd_out)
{
	*fd_in = open(argv[1], O_RDONLY);
	if (*fd_in < 0)
	{
		perror(argv[1]);
		*fd_in = open("/dev/null", O_RDONLY);
		if (*fd_in < 0)
			return (1);
	}
	*fd_out = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*fd_out < 0)
	{
		perror(argv[argc - 1]);
		close(*fd_in);
		return (1);
	}
	return (0);
}

int	open_files(char **argv, int argc, int *fd_in, int *fd_out)
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		return (open_here_doc(argv, argc, fd_in, fd_out));
	else
		return (open_infile_outfile(argv, argc, fd_in, fd_out));
}

void	fill_cmd_args(t_exec *cmd, char **argv, int start)
{
	int	i;

	i = 0;
	while (i < cmd->nb_cmd)
	{
		cmd->cmd_args[i] = argv[start + i];
		i++;
	}
}

t_exec	*prepare_exec(char **argv, int argc)
{
	t_exec	*cmd;
	int		start;

	cmd = malloc(sizeof(t_exec));
	if (!cmd)
		return (NULL);
	cmd->cmd_path = NULL;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		cmd->nb_cmd = argc - 4;
		start = 3;
	}
	else
	{
		cmd->nb_cmd = argc - 3;
		start = 2;
	}
	cmd->cmd_args = malloc(sizeof(char *) * (cmd->nb_cmd + 1));
	if (!cmd->cmd_args)
	{
		free(cmd);
		return (NULL);
	}
	fill_cmd_args(cmd, argv, start);
	return (cmd->cmd_args[cmd->nb_cmd] = NULL, cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_exec	*cmd;
	int		fd_in;
	int		fd_out;

	if (argc < 5)
		return (1);
	if (open_files(argv, argc, &fd_in, &fd_out))
		return (1);
	cmd = prepare_exec(argv, argc);
	if (!cmd)
		return (1);
	run_pipes(cmd, fd_in, fd_out, envp);
	close(fd_in);
	close(fd_out);
	free_cmd(cmd);
	get_next_line(-42);
	return (0);
}
