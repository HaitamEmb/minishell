#include "minishell.h"

static t_inout_fds *new_inout(void)
{
	t_inout_fds *io;

	io = malloc(sizeof(t_inout_fds));
	if (!io)
		return (NULL);
	io->infile = NULL;
	io->outfile = NULL;
	io->append = false;
	io->heredoc_del = NULL;
	io->heredoc_quotes = false;
	io->fd_in = -1;
	io->fd_out = -1;
	io->stdin_backup = -1;
	io->stdout_backup = -1;
	return (io);
}

t_command *lst_new_cmd(bool pipe_out)
{
	t_command *cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->pipe_out = pipe_out;
	cmd->pipe_fd = NULL;
	cmd->inout_fds = new_inout();
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

void lst_add_back_cmd(t_command **head, t_command *new_cmd)
{
	t_command *last;

	if (!head || !new_cmd)
		return;
	if (!*head)
	{
		*head = new_cmd;
		return;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_cmd;
	new_cmd->prev = last;
}

t_command *lst_last_cmd(t_command *cmd)
{
	if (!cmd)
		return (NULL);
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}

void clear_cmd_list(t_command **head)
{
	t_command *tmp;

	if (!head)
		return;
	while (*head)
	{
		tmp = (*head)->next;
		if ((*head)->pipe_fd)
		{
			if ((*head)->pipe_fd[0] >= 0)
				close((*head)->pipe_fd[0]);
			if ((*head)->pipe_fd[1] >= 0)
				close((*head)->pipe_fd[1]);
			free((*head)->pipe_fd);
		}
		free_str_tab((*head)->args);
		if ((*head)->inout_fds)
		{
			free_ptr((*head)->inout_fds->infile);
			free_ptr((*head)->inout_fds->outfile);
			free_ptr((*head)->inout_fds->heredoc_del);
			free((*head)->inout_fds);
		}
		free_ptr((*head)->command);
		free_ptr((*head)->path);
		free(*head);
		*head = tmp;
	}
}
