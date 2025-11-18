#include "minishell.h"

int	create_token(char *str, t_data *data)
{
	//we take the user input and filter between commands and words
	//following the quotting rules in bash.
	
	int	i;
	int	end;
	int	start;
	int	status; //for quoting rules
	
	i = 0;
	start = 0;
	end = strlen(str); //change with ft_strlen
	status = DEFAULT;
	while(i <= end)
	{
		status = set_status(status, str, i);
		if (status == DEFAULT)
			start = word_or_command(&i, str, start, data);
		i++;
	}
	if (status != DEFAULT)
	{
		if (status = DOUBLE)
			//errmsg
		else if (status == SINGLE)
			//errmsg
		//errmsg
		return (1);
	}
	return (0);
}