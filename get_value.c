#include "minishell.h"
//recover values, we search var if exists from set envs or shell state and return it

static char *get_env_var(t_data *data, char *var)
{
	char *str;
	int i;
	int len;

	i = 0;
	len = ft_strlen(var);
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, len) == 0)
			break;
		i++;
	}
	str = ft_strdup(data->env[i] + len);
	return (str);
}

static int var_exists(t_data *data, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, len) == 0)
			return (0);
		i++;
	}
	return (i);
}

char *recover_val(t_token *token, char *str, t_data *data)
{
	char	*value;
	char	*var;

	var = var_id(str);
	if (var && var_exists(data, var) == 0)
	{
		if (token != NULL)
			token->is_var_exist = true;
		value = get_env_var(data, var);
	}
	else if (var && var[0] == '?' && var[1] == '=')
		value = ft_itoa(exit_code); //to fix
	else
		value = NULL;
	free_ptr(var); //to add
	return (value);
}