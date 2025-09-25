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

char	*build_and_check(char **argv, int cmd_index, char **envp)
{
	char	**cmd;
	char	*result;

	if (!argv[cmd_index] || argv[cmd_index][0] == '\0')
		return (ft_putstr_fd("Invalid command\n", 2), NULL);
	cmd = cmd_split(argv, cmd_index);
	if (!cmd || !cmd[0])
	{
		if (cmd && !cmd[0])
			free_split(cmd);
		return (NULL);
	}
	if (is_absolute_path(cmd[0]))
		result = check_absolute_path(cmd);
	else
		result = search_in_path(cmd, envp);
	free_split(cmd);
	return (result);
}
