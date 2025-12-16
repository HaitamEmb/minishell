/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:00:04 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 18:07:52 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	get_heredoc(t_data *data, t_inout_fds *io)
{
	int		tmp_fd;
	bool	ret;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (false);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		tmp_fd = open(io->infile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		ret = fill_heredoc(data, io, tmp_fd);
		close(tmp_fd);
		if (ret)
			exit(0);
		exit(1);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (true);
	g_exit_status = 130;
	return (false);
}

static char	*get_heredoc_name(void)
{
	static int	i;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin(HEREDOC_NAME, number);
	free(number);
	i++;
	return (name);
}

static char	*get_delim(char *delim, bool *quotes)
{
	int	len;

	len = ft_strlen(delim) - 1;
	if ((delim[0] == '\"' && delim[len] == '\"') || (delim[0] == '\''
			&& delim[len] == '\''))
	{
		*quotes = true;
		return (ft_strtrim(delim, "\'\""));
	}
	return (ft_strdup(delim));
}

static bool	init_heredoc_io(t_inout_fds *io, t_token *token)
{
	if (!remove_old_file_ref(io, true))
		return (false);
	io->infile = get_heredoc_name();
	if (!io->infile)
		return (false);
	io->heredoc_del = get_delim(token->next->str, &(io->heredoc_quotes));
	if (!io->heredoc_del)
	{
		free_ptr(io->infile);
		io->infile = NULL;
		return (false);
	}
	return (true);
}

int	parse_heredoc(t_data *data, t_command **last_cmd, t_token **token_lst)
{
	t_token		*tmp;
	t_command	*cmd;
	t_inout_fds	*io;
	bool		success;

	tmp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	io = cmd->inout_fds;
	if (!init_heredoc_io(io, tmp))
		return (FAILURE);
	success = get_heredoc(data, io);
	if (success)
		io->fd_in = open(io->infile, O_RDONLY);
	else
	{
		io->fd_in = -1;
		unlink(io->infile);
		return (FAILURE);
	}
	if (tmp->next->next)
		*token_lst = tmp->next->next;
	else
		*token_lst = tmp->next;
	return (SUCCESS);
}
