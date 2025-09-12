#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n", 2);
		return (1);
	}
	fd_in = open(argv[1], O_RDONLY);
	if (fd_in < 0)
	{
		perror(argv[1]);
		fd_in = open("/dev/null", O_RDONLY);
		if (fd_in < 0)
			return (1);
	}
	fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror(argv[4]);
		close(fd_in);
		return (1);
	}
	run_pipe(fd_in, fd_out, argv, envp);
	close(fd_in);
	close(fd_out);
	return (0);
}
