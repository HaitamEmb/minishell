/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:00:00 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 15:25:53 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	open_outfile_append(t_command *cmd, char *file, char *filename)
{
	t_inout_fds	*io;

	io = cmd->inout_fds;
	if (cmd->redir_failed)
		return ;
	if (!remove_old_file_ref(io, false))
		return ;
	io->append = true;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0' && filename)
	{
		errmsg_cmd(filename, NULL, "bad redirect", false);
		cmd->redir_failed = true;
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (io->fd_out == -1)
	{
		errmsg_cmd(io->outfile, NULL, strerror(errno), false);
		cmd->redir_failed = true;
	}
}

void	parse_append(t_command **last_cmd, t_token **token_lst)
{
	t_token		*tmp;
	t_command	*cmd;

	tmp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_outfile_append(cmd, tmp->next->str, tmp->next->str_back);
	if (tmp->next->next)
		tmp = tmp->next->next;
	else
		tmp = tmp->next;
	*token_lst = tmp;
}
