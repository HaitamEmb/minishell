/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:29:44 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:29:44 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_key_len(const char *entry)
{
	int	len;

	len = 0;
	while (entry[len] && entry[len] != '=')
		len++;
	return (len);
}

char	**dup_env(char **envp)
{
	char	**copy;
	size_t	size;
	size_t	i;

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

static void	swap_entries(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	print_entry(char *entry)
{
	int	key_len;

	write(STDOUT_FILENO, "declare -x ", sizeof("declare -x ") - 1);
	if (ft_strchr(entry, '='))
	{
		key_len = env_key_len(entry);
		write(STDOUT_FILENO, entry, key_len + 1);
		write(STDOUT_FILENO, "\"", 1);
		write(STDOUT_FILENO, entry + key_len + 1,
			ft_strlen(entry) - key_len - 1);
		write(STDOUT_FILENO, "\"", 1);
	}
	else
		write(STDOUT_FILENO, entry, ft_strlen(entry));
	write(STDOUT_FILENO, "\n", 1);
}

void	print_sorted_env(char **env)
{
	char	**copy;
	size_t	i;
	size_t	j;

	copy = dup_env(env);
	if (!copy)
		return ;
	i = 0;
	while (copy[i])
	{
		j = i + 1;
		while (copy[j])
		{
			if (ft_strcmp(copy[i], copy[j]) > 0)
				swap_entries(&copy[i], &copy[j]);
			j++;
		}
		i++;
	}
	i = 0;
	while (copy[i])
		print_entry(copy[i++]);
	free_str_tab(copy);
}
