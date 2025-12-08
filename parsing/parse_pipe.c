#include "../minishell.h"

void	parse_pipe(t_command **cmd, t_token **lst)
{
	t_command	*last_cmd;

	last_cmd = lst_last_cmd(*cmd);
	last_cmd->pipe_out = true;
	lst_add_back_cmd(&last_cmd, lst_new_cmd(false));
	*lst = (*lst)->next;
}