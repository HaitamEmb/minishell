/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:30:27 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:30:27 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void close_pipeline_pipes(t_command *head)
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

static int setup_input(t_command *cmd)
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

static int setup_output(t_command *cmd)
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

static void execve_error(t_command *cmd)
{
	if (!cmd->command || cmd->command[0] == '\0')
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": : command not found\n",
			  sizeof(": : command not found\n") - 1);
	}
	else if (errno == ENOENT)
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": ", sizeof(": ") - 1);
		write(STDERR_FILENO, cmd->command, ft_strlen(cmd->command));
		write(STDERR_FILENO, ": command not found\n",
			  sizeof(": command not found\n") - 1);
	}
	else
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": ", sizeof(": ") - 1);
		write(STDERR_FILENO, cmd->command, ft_strlen(cmd->command));
		write(STDERR_FILENO, ": ", sizeof(": ") - 1);
		write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		write(STDERR_FILENO, "\n", 1);
	}
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(1);
}

static int ensure_args(t_command *cmd)
{
	if (!cmd->command)
		return (FAILURE);
	if (cmd->args)
		return (SUCCESS);
	cmd->args = malloc(sizeof(char *) * 2);
	if (!cmd->args)
		return (FAILURE);
	cmd->args[0] = ft_strdup(cmd->command);
	if (!cmd->args[0])
	{
		free(cmd->args);
		cmd->args = NULL;
		return (FAILURE);
	}
	cmd->args[1] = NULL;
	return (SUCCESS);
}

void child_execute(t_data *data, t_command *cmd)
{
	setup_child_signals();
	if (ensure_args(cmd) == FAILURE)
		exit(1);
	if (setup_input(cmd) == FAILURE || setup_output(cmd) == FAILURE)
	{
		perror("dup2");
		exit(1);
	}
	close_pipeline_pipes(data->cmd);
	close_command_fds(cmd);
	if (is_builtin(cmd->command))
		exit(exec_builtin(data, cmd, true));
	if (!cmd->command)
		exit(0);
	if (!cmd->path)
		cmd->path = resolve_command_path(data, cmd);
	if (!cmd->path)
	{
		errno = ENOENT;
		execve_error(cmd);
	}
	execve(cmd->path, cmd->args, data->env);
	execve_error(cmd);
}
