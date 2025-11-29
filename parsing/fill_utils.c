#include "minishell.h"

char **copy_in_new_tab(int len, char **n_tab, t_command *last_cmd, t_token *tmp)
{
	int i;

	i = 0;
	while (i < len)
	{
		n_tab[i] = last_cmd->args[i];
		i++;
	}
	while (tmp->type == WORD || tmp->type == VAR)
	{
		if (tmp->to_join == true)
			n_tab[i] = join_vars(&tmp);
		else
			n_tab[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	n_tab[i] = NULL;
	return (n_tab);
}

void del_empty_args(t_token **token)
{
	t_token *tmp;

	tmp = *token;
	while (tmp->type == WORD || tmp->type == VAR)
	{
		if (tmp->type == VAR && tmp->str[0] == '\0' && tmp->is_var_exist == false)
		{
			tmp = tmp->next;
			if (tmp == (*token)->next)
				(*token) = (*token)->next;
			lst_deltoken(tmp->prev, free_ptr); // add free_ptr
		}
		else
			tmp = tmp->next;
	}
}

char *join_vars(t_token **token)
{
	t_token *tmp;
	char *str;

	tmp = *token;
	str = ft_strdup(tmp->str);
	while (tmp->type == VAR && tmp->next->type == VAR && tmp->next->join == true)
	{
		str = ft_strjoin(str, tmp->next->str);
		tmp = tmp->next;
	}
	*token = tmp;
	return (str);
}