/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:30:39 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:30:39 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static int	process_heredoc_line(t_data *data, t_inout_fds *io,
		int fd, char *line)
{
	char	*expanded;

	if (!line || !ft_strcmp(line, io->heredoc_del))
	{
		free_ptr(line);
		return (1);
	}
	if (!io->heredoc_quotes)
	{
		expanded = expand_var_heredoc(data, line);
		free_ptr(line);
		line = expanded;
	}
	if (!line)
		return (0);
	write_line(fd, line);
	free_ptr(line);
	return (0);
}

static int	heredoc_loop(t_data *data, t_inout_fds *io, int *pipe_fd,
		int stdin_backup)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line && g_exit_status == 130)
		{
			dup2(stdin_backup, STDIN_FILENO);
			close(stdin_backup);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			setup_signals();
			return (FAILURE);
		}
		if (process_heredoc_line(data, io, pipe_fd[1], line))
			break ;
	}
	return (SUCCESS);
}

int	build_heredoc(t_data *data, t_inout_fds *io)
{
	int		pipe_fd[2];
	int		stdin_backup;

	if (!io || !io->heredoc_del)
		return (SUCCESS);
	if (pipe(pipe_fd) == -1)
		return (perror("heredoc"), FAILURE);
	stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
	if (heredoc_loop(data, io, pipe_fd, stdin_backup) == FAILURE)
		return (FAILURE);
	setup_signals();
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	close(pipe_fd[1]);
	if (io->fd_in >= 0)
		close(io->fd_in);
	io->fd_in = pipe_fd[0];
	return (SUCCESS);
}
