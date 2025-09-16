#include "pipex.h"

int	here_doc_input(char *limiter)
{
	int		pipefd[2];
	char	*line;
	size_t	len;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	len = ft_strlen(limiter);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
		{
			close(pipefd[1]);
			close(pipefd[0]);
			get_next_line(-42);
			return (-1);
		}
		if (len > 0 && !ft_strncmp(line, limiter, len) && line[len] == '\n')
		{
			free(line);
			break;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
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
