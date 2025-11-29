/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:29:44 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:29:44 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_assign(t_data *data, char *name, char *value)
{
	int	ret;

	if (!value)
		ret = ms_setenv(data, name, "");
	else
		ret = ms_setenv(data, name, value);
	free_ptr(name);
	return (ret);
}

static int	handle_export_error(char *arg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": export: `", sizeof(": export: `") - 1);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "' not a valid identifier\n",
		sizeof("' not a valid identifier\n") - 1);
	return (1);
}

static int	export_argument(t_data *data, char *arg)
{
	char	*equal;
	char	*name;
	char	*value;

	if (!is_valid_identifier(arg))
		return (handle_export_error(arg));
	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		if (ms_setenv(data, arg, NULL) == SUCCESS)
			return (0);
		return (1);
	}
	name = ft_substr(arg, 0, equal - arg);
	if (!name)
		return (1);
	value = equal + 1;
	if (export_assign(data, name, value) == SUCCESS)
		return (0);
	return (1);
}

int	builtin_export(t_data *data, t_command *cmd)
{
	int	status;
	int	i;

	if (!data || !cmd)
		return (1);
	if (!cmd->args || !cmd->args[1])
	{
		print_sorted_env(data->env);
		return (0);
	}
	status = 0;
	i = 1;
	while (cmd->args[i])
	{
		if (export_argument(data, cmd->args[i]))
			status = 1;
		i++;
	}
	return (status);
}

int	builtin_unset(t_data *data, t_command *cmd)
{
	int	status;
	int	i;

	if (!data || !cmd || !cmd->args)
		return (0);
	status = 0;
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
			write(STDERR_FILENO, ": unset: `", sizeof(": unset: `") - 1);
			write(STDERR_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
			write(STDERR_FILENO, "' not a valid identifier\n",
				sizeof("' not a valid identifier\n") - 1);
			status = 1;
		}
		else
			ms_unsetenv(data, cmd->args[i]);
		i++;
	}
	return (status);
}
