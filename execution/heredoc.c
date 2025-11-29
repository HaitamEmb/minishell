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

static void write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int build_heredoc(t_data *data, t_inout_fds *io)
{
	int pipe_fd[2];
	char *line;
	int stdin_backup;

	if (!io || !io->heredoc_del)
		return (SUCCESS);
	if (pipe(pipe_fd) == -1)
	{
		perror("heredoc");
		return (FAILURE);
	}
	stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
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
	setup_signals();
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	close(pipe_fd[1]);
	if (io->fd_in >= 0)
		close(io->fd_in);
	io->fd_in = pipe_fd[0];
	return (SUCCESS);
}
