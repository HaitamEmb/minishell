/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:46:32 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 21:46:32 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

static size_t count_commands(t_command *cmd)
{
	size_t count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int create_pipe_for_cmd(t_command *cmd)
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

static void close_pipe_pair(int *pipe_fd)
{
	if (!pipe_fd)
		return;
	if (pipe_fd[0] >= 0)
		close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	free(pipe_fd);
}

static void free_all_pipes(t_command *cmd)
{
	while (cmd)
	{
		close_pipe_pair(cmd->pipe_fd);
		cmd->pipe_fd = NULL;
		cmd = cmd->next;
	}
}

static int wait_children(pid_t *pids, size_t count)
{
	size_t i;
	int status;
	int ret;

	ret = SUCCESS;
	i = 0;
	while (i < count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			ret = FAILURE;
		else if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			g_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	return (ret);
}

static int execute_single_builtin(t_data *data, t_command *cmd)
{
	int status;

	if (prepare_redirections(data, cmd) == FAILURE)
	{
		g_exit_status = 1;
		return (FAILURE);
	}
	if (apply_parent_redirs(cmd->inout_fds) == FAILURE)
	{
		restore_parent_redirs(cmd->inout_fds);
		g_exit_status = 1;
		return (FAILURE);
	}
	status = exec_builtin(data, cmd, false);
	restore_parent_redirs(cmd->inout_fds);
	close_command_fds(cmd);
	return (status);
}

static int fork_pipeline(t_data *data, pid_t *pids, size_t *launched)
{
	t_command *cmd;
	pid_t pid;

	cmd = data->cmd;
	*launched = 0;
	while (cmd)
	{
		if (create_pipe_for_cmd(cmd) == FAILURE)
			return (FAILURE);
		if (prepare_redirections(data, cmd) == FAILURE)
		{
			g_exit_status = 1;
			return (FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (FAILURE);
		}
		if (pid == 0)
			child_execute(data, cmd);
		pids[*launched] = pid;
		(*launched)++;
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
		close_command_fds(cmd);
		cmd = cmd->next;
	}
	return (SUCCESS);
}

int run_execution(t_data *data)
{
	size_t count;
	pid_t *pids;
	int ret;
	size_t launched;

	if (!data || !data->cmd)
		return (SUCCESS);
	count = count_commands(data->cmd);
	if (count == 1 && is_builtin(data->cmd->command))
		return (execute_single_builtin(data, data->cmd));
	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
		return (FAILURE);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ret = fork_pipeline(data, pids, &launched);
	if (launched > 0)
		wait_children(pids, launched);
	setup_signals();
	free(pids);
	free_all_pipes(data->cmd);
	return (ret);
}
