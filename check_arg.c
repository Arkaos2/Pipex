/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saibelab <saibelab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:21:08 by saibelab          #+#    #+#             */
/*   Updated: 2025/09/13 17:21:08 by saibelab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**cmd_split(char **argv, int i)
{
	char	**args;

	args = ft_split(argv[i], ' ');
	if (!args)
		return (NULL);
	return (args);
}

char	**get_path(char **envp)
{
	int		i;
	char	*path_value;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	path_value = envp[i] + 5;
	path = ft_split(path_value, ':');
	if (!path)
		return (NULL);
	return (path);
}

char	*try_paths_for_cmd(char **paths, char **cmd)
{
	char	*tmp;
	char	*correct;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			break ;
		correct = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (!correct)
			break ;
		if (access(correct, X_OK) == 0)
			return (correct);
		free(correct);
		i++;
	}
	return (NULL);
}

char	*build_and_check(char **argv, int cmd_index, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*result;

	paths = get_path(envp);
	cmd = cmd_split(argv, cmd_index);
	if (!paths || !cmd || !cmd[0])
	{
		free_split(cmd);
		free_split(paths);
		return (NULL);
	}
	result = try_paths_for_cmd(paths, cmd);
	if (!result)
		ft_putstr_fd(": command not found\n", 2);
	free_split(cmd);
	free_split(paths);
	return (result);
}

void	free_cmd(t_exec *cmd)
{
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	if (cmd->cmd_args)
		free_split(cmd->cmd_args);
}
