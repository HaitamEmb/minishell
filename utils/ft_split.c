/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:31:51 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:31:51 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t word_len(const char *s, char c)
{
	size_t len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static size_t count_words(const char *s, char c)
{
	size_t count;
	size_t i;

	if (!s)
		return (0);
	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			i += word_len(s + i, c);
		}
	}
	return (count);
}

static void free_table(char **tab, size_t limit)
{
	while (limit > 0)
	{
		free(tab[limit - 1]);
		limit--;
	}
	free(tab);
}

char **ft_split(char const *s, char c)
{
	char **tab;
	size_t idx;
	size_t len;

	if (!s)
		return (NULL);
	tab = ft_calloc(count_words(s, c) + 1, sizeof(char *));
	if (!tab)
		return (NULL);
	idx = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			len = word_len(s, c);
			tab[idx] = ft_substr(s, 0, len);
			if (!tab[idx])
			{
				free_table(tab, idx);
				return (NULL);
			}
			idx++;
			s += len;
		}
	}
	tab[idx] = NULL;
	return (tab);
}
