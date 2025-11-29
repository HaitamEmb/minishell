#include "minishell.h"

static int open_input(t_inout_fds *io)
{
	int fd;

	if (!io || !io->infile)
		return (SUCCESS);
	fd = open(io->infile, O_RDONLY);
	if (fd < 0)
	{
		perror(io->infile);
		return (FAILURE);
	}
	if (io->fd_in >= 0)
		close(io->fd_in);
	io->fd_in = fd;
	return (SUCCESS);
}

static int open_output(t_inout_fds *io)
{
	int fd;
	int flags;

	if (!io || !io->outfile)
		return (SUCCESS);
	flags = O_CREAT | O_WRONLY;
	if (io->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(io->outfile, flags, 0644);
	if (fd < 0)
	{
		perror(io->outfile);
		return (FAILURE);
	}
	if (io->fd_out >= 0)
		close(io->fd_out);
	io->fd_out = fd;
	return (SUCCESS);
}

int prepare_redirections(t_data *data, t_command *cmd)
{
	t_inout_fds *io;

	(void)data;
	if (!cmd)
		return (SUCCESS);
	io = cmd->inout_fds;
	if (!io)
		return (SUCCESS);
	if (io->heredoc_del)
	{
		if (build_heredoc(data, io) == FAILURE)
			return (FAILURE);
	}
	else if (io->infile && open_input(io) == FAILURE)
		return (FAILURE);
	if (io->outfile && open_output(io) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int apply_parent_redirs(t_inout_fds *io)
{
	if (!io)
		return (SUCCESS);
	if (io->fd_in >= 0)
	{
		if (io->stdin_backup == -1)
			io->stdin_backup = dup(STDIN_FILENO);
		if (dup2(io->fd_in, STDIN_FILENO) == -1)
			return (FAILURE);
	}
	if (io->fd_out >= 0)
	{
		if (io->stdout_backup == -1)
			io->stdout_backup = dup(STDOUT_FILENO);
		if (dup2(io->fd_out, STDOUT_FILENO) == -1)
			return (FAILURE);
	}
	return (SUCCESS);
}

void restore_parent_redirs(t_inout_fds *io)
{
	if (!io)
		return;
	if (io->stdin_backup != -1)
	{
		dup2(io->stdin_backup, STDIN_FILENO);
		close(io->stdin_backup);
		io->stdin_backup = -1;
	}
	if (io->stdout_backup != -1)
	{
		dup2(io->stdout_backup, STDOUT_FILENO);
		close(io->stdout_backup);
		io->stdout_backup = -1;
	}
}

void close_command_fds(t_command *cmd)
{
	if (!cmd || !cmd->inout_fds)
		return;
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
