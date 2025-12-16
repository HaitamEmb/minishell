/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 02:00:40 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 11:59:03 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word(t_token **token_lst, char *str, int index, int start)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(sizeof(char) * (index - start + 1));
	if (!word)
		return (1);
	while (start < index)
	{
		word[i] = str[start];
		start++;
		i++;
	}
	word[i] = '\0';
	lst_add_prev(token_lst, lst_new_token(word, ft_strdup(word), WORD,
			DEFAULT));
	return (0);
}

static int	command(t_token **head, char *str, int idx, int type)
{
	int		i;
	char	*cmd;

	i = 0;
	if (type == APPEND || type == HEREDOC)
	{
		cmd = malloc(sizeof(char) * 3);
		if (!cmd)
			return (1);
		while (i < 2)
			cmd[i++] = str[idx++];
		cmd[i] = '\0';
		lst_add_prev(head, lst_new_token(cmd, NULL, type, DEFAULT));
	}
	else
	{
		cmd = malloc(sizeof(char) * 2);
		if (!cmd)
			return (1);
		while (i < 1)
			cmd[i++] = str[idx++];
		cmd[i] = '\0';
		lst_add_prev(head, lst_new_token(cmd, NULL, type, DEFAULT));
	}
	return (0);
}

int	is_command(char *str, int i)
{
	if ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		return (TOKEN_SPACE);
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

int	set_status(int status, char *str, int i)
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

int	word_or_command(int *i, char *str, int start, t_data *data)
{
	int	type;

	type = is_command(str, (*i));
	if (type)
	{
		if ((*i) != 0 && is_command(str, (*i) - 1) == 0)
			word(&data->token, str, (*i), start);
		if (type == APPEND || type == HEREDOC || type == PIPE || type == INPUT
			|| type == OUTPUT || type == END)
		{
			command(&data->token, str, (*i), type);
			if (type == APPEND || type == HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}
