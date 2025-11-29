#include "minishell.h"

int is_command(char *str, int i)
{
	if ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		return (SPACE);
	else if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '<')
		return (INPUT);
	else if (str[i] == '>')
		return (OUTPUT);
	else if (str[i] == '\0')
		return (END);
	else
		return (0);
}

int set_status(int status, char *str, int i)
{
	if (str[i] == '\'' && status == DEFAULT)
		status = SINGLE;
	else if (str[i] == '\"' && status == DEFAULT)
		status = DOUBLE;
	else if (str[i] == '\'' && status == SINGLE)
		status = DEFAULT;
	else if (str[i] == '\"' && status == DOUBLE)
		status = DEFAULT;
	return (status);
}

int word_or_command(int *i, char *str, int start, t_data *data)
{
	int type;

	type = is_command(str, (*i));
	if (type)
	{
		if ((*i) != 0 && is_command(str, (*i) - 1) == 0)
			word(); // add later
		if (type == APPEND || type == HEREDOC || type == PIPE || type == INPUT || type == OUTPUT || type == END)
		{
			command(); // add later
			if (type == APPEND || type == HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}
