/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:46 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 11:59:03 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_str_len(char *str, int count, int i)
{
	int	status;

	status = -1;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && status == DEFAULT)
		{
			if (str[i] == '\'')
				status = SINGLE;
			if (str[i] == '\"')
				status = DOUBLE;
			i++;
			continue ;
		}
		else if ((str[i] == '\'' && status == SINGLE) || (str[i] == '\"'
				&& status == DOUBLE))
		{
			status = DEFAULT;
			i++;
			continue ;
		}
		count++;
		i++;
	}
	return (count + 1);
}

bool	str_has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

int	handle_quotes(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		if (str_has_quotes(tmp->str) == true && (!tmp->prev
				|| (tmp->prev->type != HEREDOC)))
			remove_quotes(&tmp);
		tmp = tmp->next;
	}
	return (SUCCESS);
}
