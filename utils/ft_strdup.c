#include "minishell.h"

char *ft_strdup(const char *s)
{
	char *copy;
	size_t len;
	size_t i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
