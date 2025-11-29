/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_basic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:29:58 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:29:58 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echo_has_only_n(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (arg[i] != 'n')
		return (0);
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	suppress_newline;

	if (!cmd || !cmd->args)
		return (SUCCESS);
	i = 1;
	suppress_newline = 0;
	while (cmd->args[i] && echo_has_only_n(cmd->args[i]))
	{
		suppress_newline = 1;
		i++;
	}
	while (cmd->args[i])
	{
		write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!suppress_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (SUCCESS);
}

int	builtin_pwd(void)
{
	char	buffer[PATH_MAX];
	char	*cwd;

	cwd = getcwd(buffer, PATH_MAX);
	if (!cwd)
	{
		perror("pwd");
		return (FAILURE);
	}
	write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	return (SUCCESS);
}

int	builtin_env(t_data *data)
{
	int	i;

	if (!data || !data->env)
		return (SUCCESS);
	i = 0;
	while (data->env[i])
	{
		if (ft_strchr(data->env[i], '='))
		{
			write(STDOUT_FILENO, data->env[i], ft_strlen(data->env[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	return (SUCCESS);
}
