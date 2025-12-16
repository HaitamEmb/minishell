/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_trunc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:00:18 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 15:25:53 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_relative_path(char *file)
{
	char	*path;
	char	*ret;

	if (file[0] == '/')
		return (ft_strdup(file));
	path = ft_strdup("./");
	ret = ft_strjoin(path, file);
	printf("Parsing, relative path is : %s\n", ret);
	return (ret);
}

static void	open_outfile(t_command *cmd, char *file, char *filename)
{
	t_inout_fds	*io;

	io = cmd->inout_fds;
	if (cmd->redir_failed)
		return ;
	if (!remove_old_file_ref(io, false))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0')
	{
		errmsg_cmd(filename, NULL, "bad redirect", false);
		cmd->redir_failed = true;
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (io->fd_out == -1)
	{
		errmsg_cmd(io->outfile, NULL, strerror(errno), false);
		cmd->redir_failed = true;
	}
}

void	parse_trunc(t_command **last_cmd, t_token **token_lst)
{
	t_token		*tmp;
	t_command	*cmd;

	tmp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_outfile(cmd, tmp->next->str, tmp->next->str_back);
	if (tmp->next->next)
		tmp = tmp->next->next;
	else
		tmp = tmp->next;
	*token_lst = tmp;
}
