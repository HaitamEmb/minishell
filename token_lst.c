#include "minishell.h"

t_token *lst_new_token(char *str, char *str_back, int type, int status)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->str = str;
	new_token->str_back = str_back;
	new_token->is_var_exist = false;
	new_token->status = status;
	new_token->to_join = false;
	new_token->prev = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	lst_add_prev(t_token **head, t_token *new_token)
{
	t_token *start;

	start = *head;
	if (start == NULL)
	{
		*head = new_token;
		return;
	}
	if (head && *head && new_token)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_token;
		new_token->prev = start;
	}
}

void	lst_deltoken(t_token *token, void (*del)(void *))
{
	if (del && token && token->str)
	{
		(*del)(token->str);
		token->str = NULL;
	}
	if (del && token && token->str_back)
	{
		(*del)(token->str_back);
		token->str_back = NULL;
	}
	if (token->prev)
		token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	free_ptr(token); //add freeptr
}

void	lst_clear(t_token **head, void (*del)(void *))
{
	t_token *tmp;
	tmp = NULL;
	while (*head != NULL)
	{
		tmp = (*head)->next;
		lst_deltoken(*head, del);
		*head = tmp;
	}
}