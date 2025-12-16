/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:14 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 12:09:02 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_token(char *str, t_data *data)
{
	int	i;
	int	end;
	int	start;
	int	status;

	i = 0;
	start = 0;
	end = ft_strlen(str);
	status = DEFAULT;
	while (i <= end)
	{
		status = set_status(status, str, i);
		if (status == DEFAULT)
			start = word_or_command(&i, str, start, data);
		i++;
	}
	if (status != DEFAULT)
	{
		if (status == DOUBLE)
			errmsg("syntax error", "unclosed double quote", 1);
		else if (status == SINGLE)
			errmsg("syntax error", "unclosed single quote", 1);
		return (FAILURE);
	}
	return (SUCCESS);
}
