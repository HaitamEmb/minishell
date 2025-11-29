#include "minishell.h"
// extarct variable names from str

bool is_var_valid(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}
int var_len(char *str)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while (str[i] != '$')
		i++;
	i++;
	if (ft_isnum(str[i]) || str[i] == '?')
		return (count + 1);
	while (str[i])
	{
		if (is_var_valid(str[i]) == false)
			break;
		count++;
		i++;
	}
	return (count);
}
// find the $ and get the var name
char *var_id(char *str)
{
	char *var;
	char *tmp;
	int start;
	int len;
	int i;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			start = i + 1;
			break;
		}
		i++;
	}
	len = var_len(str);
	var = ft_substr(str, start, len);
	if (!var)
		return (NULL);
	tmp = ft_strjoin(var, "=");
	free_ptr(var);
	var = tmp;
	return (var);
}
