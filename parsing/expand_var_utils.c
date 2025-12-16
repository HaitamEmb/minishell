/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:20 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 11:59:03 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_to_new_str(char *n_str, char *var_val, int *j)
{
	int	k;

	k = 0;
	while (var_val[k])
	{
		n_str[*j] = var_val[k];
		k++;
		(*j)++;
	}
}

char	*create_token_string(char *old_str, char *var_val, int n_size, int idx)
{
	int		i;
	int		j;
	char	*n_str;

	i = 0;
	j = 0;
	n_str = malloc(sizeof(char) * n_size);
	if (!n_str)
		return (NULL);
	while (old_str[i])
	{
		if (old_str[i] == '$' && i == idx)
		{
			copy_to_new_str(n_str, var_val, &j);
			i = i + var_len(old_str + idx) + 1;
			if (old_str[i] == '\0')
				break ;
		}
		n_str[j++] = old_str[i++];
	}
	n_str[j] = '\0';
	return (n_str);
}