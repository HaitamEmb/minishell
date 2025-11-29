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

static void	print_exec_error(char *cmd, char *msg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", 2);
	if (cmd)
	{
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

static void	execve_error(t_command *cmd)
{
	if (!cmd->command || cmd->command[0] == '\0')
		print_exec_error(NULL, ": command not found");
	else if (errno == ENOENT)
		print_exec_error(cmd->command, "command not found");
	else
		print_exec_error(cmd->command, strerror(errno));
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(1);
}

static int	ensure_args(t_command *cmd)
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

void	child_execute(t_data *data, t_command *cmd)
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
