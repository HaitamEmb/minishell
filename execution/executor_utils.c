/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:46:32 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 21:46:32 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_commands(t_command *cmd)
{
	size_t	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	close_pipe_pair(int *pipe_fd)
{
	if (!pipe_fd)
		return ;
	if (pipe_fd[0] >= 0)
		close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	free(pipe_fd);
}

void	free_all_pipes(t_command *cmd)
{
	while (cmd)
	{
		close_pipe_pair(cmd->pipe_fd);
		cmd->pipe_fd = NULL;
		cmd = cmd->next;
	}
}

void	close_pipeline_pipes(t_command *head)
{
	while (head)
	{
		if (head->pipe_fd)
		{
			if (head->pipe_fd[0] >= 0)
				close(head->pipe_fd[0]);
			if (head->pipe_fd[1] >= 0)
				close(head->pipe_fd[1]);
		}
		head = head->next;
	}
}

void	close_command_fds(t_command *cmd)
{
	if (!cmd || !cmd->inout_fds)
		return ;
	if (cmd->inout_fds->fd_in >= 0)
	{
		close(cmd->inout_fds->fd_in);
		cmd->inout_fds->fd_in = -1;
	}
	if (cmd->inout_fds->fd_out >= 0)
	{
		close(cmd->inout_fds->fd_out);
		cmd->inout_fds->fd_out = -1;
	}
}
