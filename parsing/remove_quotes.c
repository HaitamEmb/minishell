#include "minishell.h"
// remove quotes while respecting quoting rules

static bool change_status_default(t_token **token, int *i)
{
	if (((*token)->str[*i] == '\'' && (*token)->status == SINGLE) || ((*token)->str[*i] == '\"' && (*token)->status == DOUBLE))
	{
		(*token)->status = DEFAULT;
		(*i)++;
		return (true);
	}
	else
		return (false);
}

static bool quote_default(t_token **token, int i)
{
	if ((*token)->str[i] == '\'' || (*token)->str[i] == '\"' && (*token)->status == DEFAULT)
		return (true);
	else
		return (false);
}

static void change_status_quote(t_token **token, int *i)
{
	if ((*token)->str[*i] == '\'')
		(*token)->status = SINGLE;
	if ((*token)->str[*i] == '\"')
		(*token)->status = DOUBLE;
	(*i)++;
}

int remove_quotes(t_token **token)
{
	char *line;
	int i;
	int j;

	i = 0;
	j = 0;
	line = malloc(sizeof(char) * quote_str_len((*token)->str, i, i));
	if (!line)
		return (1);
	while ((*token)->str[i])
	{
		if (quote_default(token, i) == true)
		{
			change_status_quote(token, &i);
			continue;
		}
		else if (change_status_default(token, &i) == true)
			continue;
		line[j++] = (*token)->str[i++];
	}
	line[j] = '\0';
	free_ptr((*token)->str);
	(*token)->str = line;
	(*token)->to_join = true;
	return (0);
}