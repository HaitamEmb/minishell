/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 01:59:36 by helmouta          #+#    #+#             */
/*   Updated: 2025/12/16 12:14:43 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*arr_to_str(char **arr)
{
	char	*str;
	char	*tmp;
	int		i;

	str = NULL;
	i = -1;
	while (arr[++i])
	{
		tmp = str;
		if (i == 0)
			str = ft_strdup(arr[0]);
		else
		{
			str = ft_strjoin(tmp, arr[i]);
			free_ptr(tmp);
		}
		if (arr[i + 1])
		{
			tmp = str;
			str = ft_strjoin(tmp, " ");
			free_ptr(tmp);
		}
	}
	free_str_tab(arr);
	return (str);
}

static char	*var_line_expand(t_data *data, char *line)
{
	char	**words;
	char	*tmp;
	int		i;

	i = 0;
	words = ft_split(line, ' ');
	if (!words)
		return (NULL);
	while (words[i])
	{
		if (ft_strchr(words[i], '$'))
		{
			tmp = words[i];
			words[i] = expand_var_heredoc(data, words[i]);
			free_ptr(tmp);
			if (!words[i])
			{
				free_str_tab(words);
				return (NULL);
			}
		}
		i++;
	}
	return (arr_to_str(words));
}

static bool	eval_heredoc(t_data *data, char **line, t_inout_fds *io, bool *ret)
{
	char	*tmp;

	if (*line == NULL)
	{
		*ret = false;
		return (false);
	}
	if (ft_strcmp(*line, io->heredoc_del) == 0)
	{
		*ret = true;
		return (false);
	}
	if (io->heredoc_quotes == false && ft_strchr(*line, '$'))
	{
		tmp = *line;
		*line = var_line_expand(data, *line);
		free_ptr(tmp);
		if (!(*line))
		{
			*ret = false;
			*line = NULL;
			return (false);
		}
	}
	return (true);
}

static bool	check_line(char **line, bool *ret)
{
	*line = readline("> ");
	if (!*line)
	{
		if (g_exit_status == 130)
		{
			*ret = false;
			return (false);
		}
		errmsg("warning", "here-document delimited by end-of-file", 0);
		*ret = true;
		return (false);
	}
	return (true);
}

bool	fill_heredoc(t_data *data, t_inout_fds *io, int fd)
{
	char	*line;
	bool	ret;

	ret = false;
	line = NULL;
	setup_heredoc_signals();
	while (1)
	{
		if (!check_line(&line, &ret))
			break ;
		if (!eval_heredoc(data, &line, io, &ret))
			break ;
		ft_putendl_fd(line, fd);
		free_ptr(line);
	}
	free_ptr(line);
	setup_signals();
	return (ret);
}
