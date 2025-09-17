/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saibelab <saibelab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:25:03 by saibelab          #+#    #+#             */
/*   Updated: 2025/09/17 15:25:03 by saibelab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	process_heredoc_line(char *line, char *limiter, int pipefd)
{
	size_t	len;

	len = ft_strlen(limiter);
	if (len > 0 && !ft_strncmp(line, limiter, len) && line[len] == '\n')
	{
		free(line);
		return (1);
	}
	write(pipefd, line, ft_strlen(line));
	free(line);
	return (0);
}

static void	cleanup_heredoc(int *pipefd)
{
	close(pipefd[1]);
	close(pipefd[0]);
	get_next_line(-42);
}

int	here_doc_input(char *limiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
		{
			cleanup_heredoc(pipefd);
			return (-1);
		}
		if (process_heredoc_line(line, limiter, pipefd[1]))
			break ;
	}
	return (close(pipefd[1]), pipefd[0]);
}

int	open_here_doc(char **argv, int argc, int *fd_in, int *fd_out)
{
	*fd_in = here_doc_input(argv[2]);
	if (*fd_in < 0)
	{
		*fd_in = open("/dev/null", O_RDONLY);
		if (*fd_in < 0)
			return (1);
	}
	*fd_out = open(argv[argc - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (*fd_out < 0)
	{
		perror(argv[argc - 1]);
		close(*fd_in);
		return (1);
	}
	return (0);
}
