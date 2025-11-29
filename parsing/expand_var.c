// we take the token VAR we either expand it if its between double quotes or not.
#include "minishell.h"

static void update_local_status(t_token **token, char c)
{
	if (c == '\'' && (*token)->status == DEFAULT)
		(*token)->status = SINGLE;
	else if (c == '\"' && (*token)->status == DEFAULT)
		(*token)->status = DOUBLE;
	else if (c == '\'' && (*token)->status == SINGLE)
		(*token)->status = DEFAULT;
	else if (c == '\"' && (*token)->status == DOUBLE)
		(*token)->status = DEFAULT;
}

static bool is_next_cmd(char c)
{
	if (c == '$' || c == ' ' || c == '=' || c == '\0')
		return (true);
	else
		return (false);
}

static bool quoted_var(char *str, int i)
{
	if (i > 0)
	{
		if (str[i - 1] == '\"' && str[i + 1] == '\"')
			return (true);
		else
			return (false);
	}
	return (false);
}

int expand_variables(t_data *data, t_token **token)
{
	t_token *tmp;
	int i;

	tmp = *token;
	while (tmp)
	{
		if (tmp->type == VAR)
		{
			i = 0;
			while (tmp->str[i])
			{
				update_local_status(&tmp, tmp->str[i]);
				if (tmp->str[i] == '$' && quoted_var(tmp->str, i) == false && is_next_cmd(tmp->str[i + 1]) == false && (tmp->status == DEFAULT || tmp->status == DOUBLE))
					replace_var(&tmp, recover_val(tmp, tmp->str + i, data), i);
				else
					i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

char *expand_var_heredoc(t_data *data, char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_next_cmd(str[i + 1]) == false && quoted_var(str, i) == false)
			str = replace_heredoc_var(str, recover_val(NULL, str + i, data), i);
		else
			i++;
	}
	return (str);
}
