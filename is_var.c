#include "minishell.h"

void	check_var(t_token **token)
{
	int i;

	i = 0;
	while ((*token)->str[i])
	{
		if ((*token)->str[i] == '$')
		{
			if ((*token)->prev && (*token)->prev->type == HEREDOC)
				break;
			(*token)->type = VAR;
			return ;
		}
		i++;
	}
}

int	is_var(t_token **lst_token)
{
	t_token *tmp;

	tmp = *lst_token;
	if (tmp->type == PIPE)
	{
		//errmsg()
		return (FAILURE);
	}
	while (tmp)
	{
		check_var(&tmp);
		if (is_invalid_ops(&tmp) == FAILURE) //create later
			return (FAILURE);
		tmp = tmp->next;
	}
	return (SUCCESS);
}