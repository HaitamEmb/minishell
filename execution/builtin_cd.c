/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:26:46 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:26:46 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int update_pwd(t_data *data, const char *oldpwd)
{
	char buffer[PATH_MAX];
	char *cwd;

	cwd = getcwd(buffer, PATH_MAX);
	if (!cwd)
		return (FAILURE);
	if (oldpwd)
		ms_setenv(data, "OLDPWD", oldpwd);
	return (ms_setenv(data, "PWD", cwd));
}

static int cd_error(const char *path)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": cd: ", sizeof(": cd: ") - 1);
	write(STDERR_FILENO, path, ft_strlen(path));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

int builtin_cd(t_data *data, t_command *cmd)
{
	char *target;
	char *home;
	char oldpwd[PATH_MAX];

	if (!data || !cmd)
		return (FAILURE);
	if (!getcwd(oldpwd, PATH_MAX))
		oldpwd[0] = '\0';
	if (!cmd->args || !cmd->args[1])
	{
		home = ms_getenv(data, "HOME");
		if (!home)
		{
			write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
			write(STDERR_FILENO, ": cd: HOME not set\n",
				  sizeof(": cd: HOME not set\n") - 1);
			return (1);
		}
		target = home;
	}
	else
		target = cmd->args[1];
	if (cmd->args && cmd->args[1] && cmd->args[2])
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": cd: too many arguments\n",
			  sizeof(": cd: too many arguments\n") - 1);
		return (1);
	}
	if (chdir(target) == -1)
	{
		if (!cmd->args || !cmd->args[1])
			free_ptr(target);
		return (cd_error(target));
	}
	update_pwd(data, oldpwd);
	if (!cmd->args || !cmd->args[1])
		free_ptr(target);
	return (0);
}
