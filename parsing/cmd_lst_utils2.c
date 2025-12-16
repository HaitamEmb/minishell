/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_lst_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:58:56 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 11:59:03 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_fds(t_command *cmd)
{
	if (cmd->pipe_fd)
	{
		if (cmd->pipe_fd[0] >= 0)
			close(cmd->pipe_fd[0]);
		if (cmd->pipe_fd[1] >= 0)
			close(cmd->pipe_fd[1]);
		free(cmd->pipe_fd);
	}
	if (cmd->inout_fds)
	{
		if (cmd->inout_fds->fd_in >= 0 && cmd->inout_fds->fd_in != STDIN_FILENO)
			close(cmd->inout_fds->fd_in);
		if (cmd->inout_fds->fd_out >= 0
			&& cmd->inout_fds->fd_out != STDOUT_FILENO)
			close(cmd->inout_fds->fd_out);
		if (cmd->inout_fds->infile && cmd->inout_fds->heredoc_del)
			unlink(cmd->inout_fds->infile);
		free_ptr(cmd->inout_fds->infile);
		free_ptr(cmd->inout_fds->outfile);
		free_ptr(cmd->inout_fds->heredoc_del);
		free(cmd->inout_fds);
	}
}

void	clear_cmd_list(t_command **head)
{
	t_command	*tmp;

	if (!head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		close_fds(*head);
		free_str_tab((*head)->args);
		free_ptr((*head)->command);
		free_ptr((*head)->path);
		free(*head);
		*head = tmp;
	}
}
