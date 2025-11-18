//checks for invalid operators format, for example double || or triple redirect >>>
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
			if (tmp->type == END &&  tmp->prev && tmp->prev->type > PIPE)
				//errmsg
			else if (tmp->type == END && tmp->prev)
				//errmsg
			else
				//errmsg
			return (FAILURE);
		}
		tmp = tmp->next;
	}
	return (SUCCESS);
}