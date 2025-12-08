#include "../minishell.h"

void	exit_minishell(t_data *data, int exitno)
{
	if (data)
	{
		if (data->cmd && data->cmd->inout_fds)
			close_command_fds(data->cmd);
		free_data(data);
	}
	exit(exitno);
}


static char	*strs_join(char *str, char *add)
{
	char	*tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add));
	tmp = str;
	str = ft_strjoin(tmp, add);
	free_ptr(tmp);
	return (str);
}

static bool	detail_quotes(char *cmd)
{
	if (ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0)
		return (true);
	return (false);
}

int	errmsg_cmd(char *command, char *desc, char *emsg, int err_nb)
{
	char	*msg;
	bool	quotes_detail = detail_quotes(command);
	msg = ft_strdup("minishell: ");
	if (command != NULL)
	{
		msg = strs_join(msg, command);
		msg = strs_join(msg, ": ");
	}
	if (desc != NULL)
	{
		if (quotes_detail)
			msg = strs_join(msg, "`");
		msg = strs_join(msg, desc);
		if (quotes_detail)
			msg = strs_join(msg, "'");
		msg = strs_join(msg, ": ");
	}
	msg = strs_join(msg, emsg);
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
	return (err_nb);
}

void	errmsg(char *errmsg, char *detail, int quotes)
{
	char	*msg;

	msg = ft_strdup("minishell: ");
	msg = strs_join(msg, errmsg);
	if (quotes)
		msg = strs_join(msg, " `");
	else
		msg = strs_join(msg, ": ");
	msg = strs_join(msg, detail);
	if (quotes)
		msg = strs_join(msg, "'");
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
}

