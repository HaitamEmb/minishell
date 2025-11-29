#include "minishell.h"

int is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int exec_builtin(t_data *data, t_command *cmd, bool in_child)
{
	int status;

	status = 0;
	if (!ft_strcmp(cmd->command, "echo"))
		status = builtin_echo(cmd);
	else if (!ft_strcmp(cmd->command, "cd"))
		status = builtin_cd(data, cmd);
	else if (!ft_strcmp(cmd->command, "pwd"))
		status = builtin_pwd();
	else if (!ft_strcmp(cmd->command, "export"))
		status = builtin_export(data, cmd);
	else if (!ft_strcmp(cmd->command, "unset"))
		status = builtin_unset(data, cmd);
	else if (!ft_strcmp(cmd->command, "env"))
		status = builtin_env(data);
	else if (!ft_strcmp(cmd->command, "exit"))
		return (builtin_exit(data, cmd, in_child));
	if (!in_child)
		g_exit_status = status;
	return (status);
}
