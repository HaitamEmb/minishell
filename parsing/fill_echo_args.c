#include "minishell.h"

// when command is echo

int args_len_echo(t_token *tmp)
{
	int i;

	i = 0;
	while (tmp && (tmp->type == WORD || tmp->type == VAR))
	{
		if (tmp->type == VAR && tmp->to_join == true)
		{
			while (tmp->type == VAR && tmp->to_join == true)
				tmp = tmp->next;
			i++;
		}
		else
		{
			i++;
			tmp = tmp->next;
		}
	}
	return (i);
}

int create_args_echo(t_token **token, t_command *last_cmd)
{
	int nb_args;
	t_token *tmp;
	int i;

	del_empty_args(token);
	tmp = *token;
	nb_args = args_len_echo(tmp);
	last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
	if (!last_cmd->args)
		return (FAILURE);
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (tmp->type == WORD || tmp->type == VAR)
	{
		if (tmp->to_join == true)
			last_cmd->args[i] = join_vars(&tmp);
		else
			last_cmd->args[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	last_cmd->args[i] = NULL;
	*token = tmp;
	return (SUCCESS);
}

int add_args_echo(t_token **token, t_command *last_cmd)
{
	int len;
	int nb_args;
	int **n_tab;
	t_token *tmp;

	del_empty_args(token); // delete var empty
	tmp = *token;
	nb_args = args_len_echo(tmp);
	len = 0;
	while (last_cmd->args[len])
		len++;
	n_tab = malloc(sizeof(char *) * (nb_args + len + 1));
	if (!n_tab)
		return (FAILURE);
	n_tab = copy_in_new_tab(len, n_tab, last_cmd, tmp);
	free(last_cmd->args);
	last_cmd->args = n_tab;
	while (tmp->type == WORD || tmp->type == VAR)
		tmp = tmp->next;
	*token = tmp;
	return (SUCCESS);
}
