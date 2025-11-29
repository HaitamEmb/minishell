/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:30:19 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:30:19 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_new_env_array(t_data *data, char ***new_env, size_t *size)
{
	size_t	i;

	*size = 0;
	while (data->env && data->env[*size])
		(*size)++;
	*new_env = malloc(sizeof(char *) * (*size + 2));
	if (!*new_env)
		return (FAILURE);
	i = 0;
	while (i < *size)
	{
		(*new_env)[i] = data->env[i];
		i++;
	}
	return (SUCCESS);
}

int	create_env_entry(const char *name, const char *value, char **entry)
{
	char	*tmp;

	*entry = ft_strjoin(name, "=");
	if (!*entry)
		return (FAILURE);
	if (value)
	{
		tmp = ft_strjoin(*entry, value);
		free_ptr(*entry);
		*entry = tmp;
	}
	if (!*entry)
		return (FAILURE);
	return (SUCCESS);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalnum(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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

int	ms_unsetenv(t_data *data, const char *name)
{
	int	idx;

	if (!data || !data->env || !name)
		return (FAILURE);
	idx = find_env_index(data->env, name);
	if (idx < 0)
		return (SUCCESS);
	free_ptr(data->env[idx]);
	while (data->env[idx])
	{
		data->env[idx] = data->env[idx + 1];
		idx++;
	}
	return (SUCCESS);
}
