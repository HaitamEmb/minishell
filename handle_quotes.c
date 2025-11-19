#include "minishell.h"

//handle quotes, we go through string get the length of the word, strip it out of quotes.

int	quote_str_len(char *str, int count, int i)
{
	int	status;

	status = -1;
	while(str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && status == DEFAULT)
		{
			if (str[i] == '\'')
				status = SINGLE;
			if (str[i] == '\"')
				status = DOUBLE;
			i++;
			continue;
		}
		else if ((str[i] == '\'' && status == SINGLE) || (str[i] == '\"' && status == DOUBLE))
		{
			status = DEFAULT;
			i++;
			continue;
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


//remove quotes from each token in the token list 

int	handle_quotes(t_data *data)
{
	t_token *tmp;

	tmp = data->token;
	while (tmp)
	{
		if (str_has_quotes(tmp->str) == true && (!tmp->prev || (tmp->prev->type != HEREDOC)))
			remove_quotes(&tmp);
		tmp = tmp->next;
	}
	return (0);
}
