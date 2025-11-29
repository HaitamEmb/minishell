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

static void swap_entries(char **a, char **b)
{
	char *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void print_sorted_env(char **env)
{
	char **copy;
	size_t i;
	size_t j;

	copy = dup_env(env);
	if (!copy)
		return;
	i = 0;
	while (copy[i])
	{
		j = i + 1;
		while (copy[j])
		{
			if (ft_strcmp(copy[i], copy[j]) > 0)
				swap_entries(&copy[i], &copy[j]);
			j++;
		}
		i++;
	}
	i = 0;
	while (copy[i])
	{
		int key_len;

		write(STDOUT_FILENO, "declare -x ", sizeof("declare -x ") - 1);
		if (ft_strchr(copy[i], '='))
		{
			key_len = env_key_len(copy[i]);
			write(STDOUT_FILENO, copy[i], key_len + 1);
			write(STDOUT_FILENO, "\"", 1);
			write(STDOUT_FILENO, copy[i] + key_len + 1,
				  ft_strlen(copy[i]) - key_len - 1);
			write(STDOUT_FILENO, "\"", 1);
		}
		else
			write(STDOUT_FILENO, copy[i], ft_strlen(copy[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	free_str_tab(copy);
}

static int export_assign(t_data *data, char *name, char *value)
{
	int ret;

	if (!value)
		ret = ms_setenv(data, name, "");
	else
		ret = ms_setenv(data, name, value);
	free_ptr(name);
	return (ret);
}

static int export_argument(t_data *data, char *arg)
{
	char *equal;
	char *name;
	char *value;

	if (!is_valid_identifier(arg))
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": export: `", sizeof(": export: `") - 1);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, "' not a valid identifier\n",
			  sizeof("' not a valid identifier\n") - 1);
		return (1);
	}
	equal = ft_strchr(arg, '=');
	if (!equal)
		return (ms_setenv(data, arg, NULL) == SUCCESS ? 0 : 1);
	name = ft_substr(arg, 0, equal - arg);
	if (!name)
		return (1);
	value = equal + 1;
	return (export_assign(data, name, value) == SUCCESS ? 0 : 1);
}

int builtin_export(t_data *data, t_command *cmd)
{
	int status;
	int i;

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
		status |= export_argument(data, cmd->args[i]);
		i++;
	}
	return (status != 0);
}

int builtin_unset(t_data *data, t_command *cmd)
{
	int status;
	int i;

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
