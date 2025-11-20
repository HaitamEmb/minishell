#include "minishell.h"

static bool	str_has_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (true);
		i++;
	}
	return (false);
}

static void	var_to_command(t_command *last_cmd, char *cmd_str)
{
	t_token	*n_token;
	t_token	*tmp;
	char	**arr_str;
	int	i;

	n_token = NULL;
	arr_str = ft_split(cmd_str, ' ');
	if (!arr_str)
		return ;
	last_cmd->command = ft_strdup(arr_str[0]);
	if (arr_str[1])
		n_token = lst_new_token(ft_strdup(arr_str[1]), NULL, WORD, DEFAULT);
	tmp = n_token;
	i = 1;
	while (arr_str[++i])
		lst_add_prev(&n_token, lst_new_token(ft_strdup(arr_str[i]), NULL, WORD, DEFAULT));
	lst_add_prev(&n_token, lst_new_token(NULL, NULL, END, DEFAULT));
	fill_args(&n_token, last_cmd); 
	lst_deltoken(&tmp, &free_ptr); //add free_ptr func
	free_str_tab(arr_str);

}

void	parse_word(t_command **cmd, t_token **token)
{
	t_token	*tmp;
	t_command	*last_cmd;

	tmp = *token;
	while (tmp->type == WORD || tmp->type == VAR)
	{
		last_cmd = lst_last_cmd(*cmd); // function to add cmd to the end of list
		if (tmp->prev == NULL || (tmp->prev && tmp->prev->type == PIPE) || last_cmd->command == NULL)
		{
			if (tmp->type == VAR && str_has_space(tmp->str))
				var_to_command(last_cmd, tmp->str); // splits vars to commands
			else
				last_cmd->command = ft_strdup(tmp->str);
			tmp = tmp->next;
		}
		else
			fill_args(&tmp, last_cmd);

	}
	*token = tmp;
}
