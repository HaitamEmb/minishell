/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:30:45 by isingara          #+#    #+#             */
/*   Updated: 2025/12/16 15:25:53 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_dir_cmd(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, cmd);
	free_ptr(tmp);
	return (result);
}

static char	*search_in_path(char **paths, const char *cmd)
{
	char	*candidate;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		candidate = join_dir_cmd(paths[i], cmd);
		if (!candidate)
			return (NULL);
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free_ptr(candidate);
		i++;
	}
	return (NULL);
}

static char	*check_direct_path(char *command)
{
	if (access(command, F_OK) == 0)
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		errno = EACCES;
		return (NULL);
	}
	errno = ENOENT;
	return (NULL);
}

char	*resolve_command_path(t_data *data, t_command *cmd)
{
	char	*path_value;
	char	**paths;
	char	*resolved;

	if (!cmd || !cmd->command || cmd->command[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd->command, '/'))
		return (check_direct_path(cmd->command));
	path_value = ms_getenv(data, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	free_ptr(path_value);
	if (!paths)
		return (NULL);
	resolved = search_in_path(paths, cmd->command);
	free_str_tab(paths);
	if (!resolved)
		errno = ENOENT;
	return (resolved);
}
