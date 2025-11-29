#include "minishell.h"

static void write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int build_heredoc(t_data *data, t_inout_fds *io)
{
	int pipe_fd[2];
	char *line;

	if (!io || !io->heredoc_del)
		return (SUCCESS);
	if (pipe(pipe_fd) == -1)
	{
		perror("heredoc");
		return (FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, io->heredoc_del))
		{
			free_ptr(line);
			break;
		}
		if (!io->heredoc_quotes)
		{
			char *expanded = expand_var_heredoc(data, line);
			free_ptr(line);
			line = expanded;
		}
		if (!line)
			continue;
		write_line(pipe_fd[1], line);
		free_ptr(line);
	}
	close(pipe_fd[1]);
	if (io->fd_in >= 0)
		close(io->fd_in);
	io->fd_in = pipe_fd[0];
	return (SUCCESS);
}
