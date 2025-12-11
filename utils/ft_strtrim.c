#include "../minishell.h"

static char	*remove_quotes_trim(char *str, char *set)
{
	int	start;
	int	end;
	int	len;

	if (!str || !set)
		return (NULL);
	start = 0;
	len = ft_strlen(str);
	while (str[start] && ft_strchr(set, str[start]))
		start++;
	end = len - 1;
	while (end > start && ft_strchr(set, str[end]))
		end--;
	return (ft_substr(str, start, end - start + 1));
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	char	*result;

	if (!s1 || !set)
		return (NULL);
	str = (char *)s1;
	result = remove_quotes_trim(str, (char *)set);
	return (result);
}
