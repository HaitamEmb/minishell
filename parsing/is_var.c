/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_var.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:57 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 11:59:03 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_var(t_token **token)
{
	int	i;

	i = 0;
	while ((*token)->str[i])
	{
		if ((*token)->str[i] == '$')
		{
			if ((*token)->prev && (*token)->prev->type == HEREDOC)
				break ;
			(*token)->type = VAR;
			return ;
		}
		i++;
	}
}

int	is_var(t_token **lst_token)
{
	t_token	*tmp;

	tmp = *lst_token;
	if (!tmp)
		return (SUCCESS);
	if (tmp->type == PIPE)
	{
		errmsg("syntax error", "unexpected token '|'", 1);
		return (FAILURE);
	}
	while (tmp)
	{
		check_var(&tmp);
		if (is_invalid_ops(&tmp) == FAILURE)
			return (FAILURE);
		tmp = tmp->next;
	}
	return (SUCCESS);
}