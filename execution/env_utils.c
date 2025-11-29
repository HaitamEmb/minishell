#include "minishell.h"

int env_key_len(const char *entry)
{
	int len;

	len = 0;
	while (entry[len] && entry[len] != '=')
		len++;
	return (len);
}

static int find_env_index(char **env, const char *name)
{
	int len;
	int i;

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

char *ms_getenv(t_data *data, const char *name)
{
	int idx;

	if (!data || !name)
		return (NULL);
	idx = find_env_index(data->env, name);
	if (idx == -1)
		return (NULL);
	return (ft_strdup(data->env[idx] + ft_strlen(name) + 1));
}

static int replace_env_entry(t_data *data, const char *name, const char *value, int idx)
{
	char *entry;
	char *tmp;

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

static int append_env_entry(t_data *data, const char *name, const char *value)
{
	char **new_env;
	char *entry;
	char *tmp;
	size_t size;
	size_t i;

	size = 0;
	while (data->env && data->env[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (FAILURE);
	i = 0;
	while (i < size)
	{
		new_env[i] = data->env[i];
		i++;
	}
	entry = ft_strjoin(name, "=");
	if (!entry)
	{
		free(new_env);
		return (FAILURE);
	}
	if (value)
	{
		tmp = ft_strjoin(entry, value);
		free_ptr(entry);
		entry = tmp;
	}
	if (!entry)
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

int ms_setenv(t_data *data, const char *name, const char *value)
{
	int idx;

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

int ms_unsetenv(t_data *data, const char *name)
{
	int idx;

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

char **dup_env(char **envp)
{
	char **copy;
	size_t size;
	size_t i;

	size = 0;
	while (envp && envp[size])
		size++;
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < size)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
				free_ptr(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[size] = NULL;
	return (copy);
}

int is_valid_identifier(const char *str)
{
	int i;

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
