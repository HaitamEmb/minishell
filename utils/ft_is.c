#include "minishell.h"

int ft_isalnum(int c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int ft_isnum(int c)
{
	return (c >= '0' && c <= '9');
}
