#include "minishell.h"

int	args_len(t_token *tmp)
{
	int	i;

	i = 0;
	while (tmp && (tmp->type == WORD || tmp->type == VAR))
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	create_args_default(t_token **token, t_command *last_cmd)
{
	int	i;
	int	nb_args;
	t_token	*tmp;

	i = 0;
	tmp = *token;
	nb_args = args_len(tmp);
	last_cmd->args = malloc(sizeof(char *) * (nb_args + 2)); //+2
	if (!last_cmd->args)
		return (FAILURE);
	tmp = *token;
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (tmp->type == WORD || tmp->type == VAR)
	{
		last_cmd->args[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	last_cmd->args[i] = NULL;
	*token = tmp;
	return (SUCCESS);
}

static char	**copy_default_in_tab(int len, char **n_tab, t_command *last_cmd, t_token **token)
{
	int	i;
	t_token	*tmp;

	i = 0;
	tmp = token;
	while (i < len)
	{
		n_tab[i] = last_cmd->args[i];
		i++;
	}
	while (tmp->type == WORD || tmp->type == VAR)
	{
		n_tab[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	n_tab[i] = NULL;
	return (n_tab);
}

int add_args_default(t_token **token, t_command *last_cmd)
{
	int	i;
	int	len;
	char	**n_tab;
	t_token	*tmp;

	i = 0;
	tmp = *token;
	while (tmp->type == WORD || tmp->type == VAR)
	{
		i++;
		tmp = tmp->next;
	}
	len = 0;
	while (last_cmd->args[len])
		len++;
	n_tab = malloc(sizeof(char *) * (i + len + 1));
	if (!n_tab)
		return (FAILURE);
	n_tab = copy_default_in_tab(len, n_tab, last_cmd, token);
	free(last_cmd->args);
	last_cmd->args = n_tab;
	*token = tmp;
	return (SUCCESS);
}

int	fill_args(t_token **token, t_command *last_cmd)
{
	if (!ft_strcmp(last_cmd->command, "echo"))
	{
		if (!(last_cmd->args))
			return (create_args_echo(token, last_cmd));
		else
			return (add_args_echo(token, last_cmd));
	}
	else
	{
		if (last_cmd && !(last_cmd->args))
			return (create_args_default(token, last_cmd));
		else
			return (add_args_default(token, last_cmd));
	}
	return (SUCCESS);
}