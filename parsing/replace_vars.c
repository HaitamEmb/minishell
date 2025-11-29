#include "minishell.h"

// take a token that has variable and either delete it if has no value, or replace with its actual value

static int del_var(t_token **token, char *str, int index)
{
	int i;
	int j;
	int len;
	char *n_str;

	i = 0;
	j = 0;
	len = ft_strlen(str) - var_len(str + index);
	n_str = (char *)malloc(sizeof(char) * len + 1);
	if (!n_str)
		return (1);
	while (str[i])
	{
		if (str[i] == '$' && i == index)
		{
			i = i + var_len(str + index) + 1;
			if (str[i] == '\0')
				break;
		}
		n_str[j++] = str[i++];
	}
	n_str[j] = '\0';
	free_ptr((*token)->str);
	(*token)->str = n_str;
	return (0);
}

static char *del_replace(t_token **token, char *str, char *var_val, int index)
{
	char *n_str;
	int n_size;

	n_size = (ft_strlen(str) - var_len(str + index)) + ft_strlen(var_val);
	n_str = create_token_string(str, var_val, n_size, index);
	if (token && *token)
	{
		free_ptr((*token)->str);
		(*token)->str = n_str;
	}
	return (n_str);
}

int replace_var(t_token **token, char *var_value, int index)
{
	if (var_value == NULL)
	{
		if (del_var(token, (*token)->str, index) == 1)
		{
			free_ptr(var_value);
			return (1);
		}
	}
	else
	{
		if (del_replace(token, (*token)->str, var_value, index) == NULL)
		{
			free_ptr(var_value);
			return (1);
		}
	}
	free_ptr(var_value);
	return (0);
}

char *replace_herdoc_var(char *str, char *var_val, int index)
{
	char *tmp;

	tmp = NULL;
	if (var_val == NULL)
		*str = '\0';
	else
	{
		tmp = str;
		str = del_replace(NULL, str, var_val, index);
		free_ptr(tmp);
	}
	free_ptr(var_val);
	return (str);
}