/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:00:36 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 12:11:10 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	double_link(t_token *to_del, t_token *tmp, t_token *insert)
{
	while (tmp != to_del)
		tmp = tmp->next;
	insert->prev = tmp->prev;
	tmp->prev->next = insert;
	while (insert->next)
		insert = insert->next;
	tmp->next->prev = insert;
	insert->next = tmp->next;
}

t_token	*lst_insert_between(t_token **head, t_token *to_del, t_token *insert)
{
	t_token	*tmp;

	tmp = *head;
	if (tmp == NULL)
		*head = insert;
	else if (tmp == to_del)
	{
		*head = insert;
		insert->next = tmp->next;
		if (tmp->next != NULL)
			tmp->next->prev = insert;
	}
	else
		double_link(to_del, tmp, insert);
	free_ptr(to_del->str);
	free_ptr(to_del);
	return (insert);
}
