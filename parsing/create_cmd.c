/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:05 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 11:59:03 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmds_with_no_args(t_data *data)
{
	t_command	*cmd;

	if (!data || !data->cmd)
		return ;
	cmd = data->cmd;
	while (cmd && cmd->command)
	{
		if (!cmd->args)
		{
			cmd->args = malloc(sizeof(*cmd->args) * 2);
			cmd->args[0] = ft_strdup(cmd->command);
			cmd->args[1] = NULL;
		}
		cmd = cmd->next;
	}
	cmd = lst_last_cmd(data->cmd);
}

void	create_cmd(t_data *data, t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->type == END)
		return ;
	while (tmp->next != NULL)
	{
		if (tmp == token)
			lst_add_back_cmd(&data->cmd, lst_new_cmd(false));
		if (tmp->type == WORD || tmp->type == VAR)
			parse_word(&data->cmd, &tmp);
		else if (tmp->type == INPUT)
			parse_input(&data->cmd, &tmp);
		else if (tmp->type == OUTPUT)
			parse_trunc(&data->cmd, &tmp);
		else if (tmp->type == HEREDOC)
			parse_heredoc(data, &data->cmd, &tmp);
		else if (tmp->type == APPEND)
			parse_append(&data->cmd, &tmp);
		else if (tmp->type == PIPE)
			parse_pipe(&data->cmd, &tmp);
		else if (tmp->type == END)
			break ;
	}
	cmds_with_no_args(data);
}
