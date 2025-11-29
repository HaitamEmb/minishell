/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:45:00 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:45:00 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe_for_cmd(t_command *cmd)
{
	if (!cmd->next)
	{
		cmd->pipe_out = false;
		return (SUCCESS);
	}
	cmd->pipe_fd = malloc(sizeof(int) * 2);
	if (!cmd->pipe_fd)
		return (FAILURE);
	if (pipe(cmd->pipe_fd) == -1)
	{
		perror("pipe");
		free(cmd->pipe_fd);
		cmd->pipe_fd = NULL;
		return (FAILURE);
	}
	cmd->pipe_out = true;
	return (SUCCESS);
}

void	handle_parent_pipes(t_command *cmd)
{
	if (cmd->prev && cmd->prev->pipe_fd)
	{
		close(cmd->prev->pipe_fd[0]);
		close(cmd->prev->pipe_fd[1]);
		free(cmd->prev->pipe_fd);
		cmd->prev->pipe_fd = NULL;
	}
	if (cmd->pipe_fd)
	{
		close(cmd->pipe_fd[1]);
		cmd->pipe_fd[1] = -1;
	}
}

int	setup_input(t_command *cmd)
{
	if (cmd->inout_fds && cmd->inout_fds->fd_in >= 0)
	{
		if (dup2(cmd->inout_fds->fd_in, STDIN_FILENO) == -1)
			return (FAILURE);
	}
	else if (cmd->prev && cmd->prev->pipe_fd)
	{
		if (dup2(cmd->prev->pipe_fd[0], STDIN_FILENO) == -1)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	setup_output(t_command *cmd)
{
	if (cmd->inout_fds && cmd->inout_fds->fd_out >= 0)
	{
		if (dup2(cmd->inout_fds->fd_out, STDOUT_FILENO) == -1)
			return (FAILURE);
	}
	else if (cmd->pipe_out && cmd->pipe_fd)
	{
		if (dup2(cmd->pipe_fd[1], STDOUT_FILENO) == -1)
			return (FAILURE);
	}
	return (SUCCESS);
}
