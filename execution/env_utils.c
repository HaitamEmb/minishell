/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:30:19 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:30:19 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_index(char **env, const char *name)
{
	int	len;
	int	i;

	if (!env || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*ms_getenv(t_data *data, const char *name)
{
	int	idx;

	if (!data || !name)
		return (NULL);
	idx = find_env_index(data->env, name);
	if (idx == -1)
		return (NULL);
	return (ft_strdup(data->env[idx] + ft_strlen(name) + 1));
}

static int	replace_env_entry(t_data *data, const char *name,
		const char *value, int idx)
{
	char	*entry;
	char	*tmp;

	entry = ft_strjoin(name, "=");
	if (!entry)
		return (FAILURE);
	if (value)
	{
		tmp = ft_strjoin(entry, value);
		free_ptr(entry);
		entry = tmp;
	}
	if (!entry)
		return (FAILURE);
	free_ptr(data->env[idx]);
	data->env[idx] = entry;
	return (SUCCESS);
}

static int	append_env_entry(t_data *data, const char *name, const char *value)
{
	char	**new_env;
	char	*entry;
	size_t	size;

	if (create_new_env_array(data, &new_env, &size) == FAILURE)
		return (FAILURE);
	if (create_env_entry(name, value, &entry) == FAILURE)
	{
		free(new_env);
		return (FAILURE);
	}
	new_env[size] = entry;
	new_env[size + 1] = NULL;
	free(data->env);
	data->env = new_env;
	return (SUCCESS);
}

int	ms_setenv(t_data *data, const char *name, const char *value)
{
	int	idx;

	if (!data || !name || name[0] == '\0')
		return (FAILURE);
	if (!data->env)
	{
		data->env = malloc(sizeof(char *));
		if (!data->env)
			return (FAILURE);
		data->env[0] = NULL;
	}
	idx = find_env_index(data->env, name);
	if (idx >= 0)
		return (replace_env_entry(data, name, value, idx));
	return (append_env_entry(data, name, value));
}
