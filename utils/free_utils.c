#include "minishell.h"

void free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
}

void free_str_tab(char **tab)
{
	size_t i;

	if (!tab)
		return;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
