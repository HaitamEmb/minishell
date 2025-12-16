/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_invalid_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helmouta <helmouta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:54 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 01:59:54 by helmouta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// checks for invalid operators format, for example double || or triple redirect >>>
#include "minishell.h"

static bool successive_ops(t_token *token)
{
	if (token->prev)
	{
		if (token->type == PIPE && token->prev->type == PIPE)
			return (true);
		if (token->type > PIPE && token->prev->type > PIPE)
			return (true);
		if (token->type == END && token->prev->type >= PIPE)
			return (true);
	}
	return (false);
}

int is_invalid_ops(t_token **token)
{
	t_token *tmp;

	tmp = *token;
	while (tmp)
	{
		if (successive_ops(tmp) == true)
		{
			if (tmp->type == END && tmp->prev && tmp->prev->type > PIPE)
				errmsg("syntax error", "unexpected token 'newline'", 1);
			else if (tmp->type == PIPE && tmp->prev && tmp->prev->type == PIPE)
				errmsg("syntax error", "unexpected token '|'", 1);
			else
				errmsg("syntax error", "unexpected token", 1);
			return (FAILURE);
		}
		tmp = tmp->next;
	}
	return (SUCCESS);
}