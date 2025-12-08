#include "../minishell.h"

//to check

static	char *arr_to_str(char **arr)
{
	char *str;
	char *tmp;
	int	i;

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

static	char *var_line_expand(t_data *data, char *line)
{
	char	**words;
	int	i;

	words = ft_split(line, ' ');
	if (!words)
		return (NULL);
	while (words[i])
	{
		if (ft_strchr(words[i], '$'))
		{
			words[i] = expand_var_heredoc(data, words[i]);
			if (!words[i])
				return (NULL);
		}
		i++;
	}
	return (arr_to_str(words));
}

static bool eval_heredoc(t_data *data, char **line, t_inout_fds *io, bool *ret)
{
	if (*line == NULL)
	{
		errmsg_cmd("");
		*ret = true;
		return (false);
	}
	if (ft_strcmp(*line, io->heredoc_del) == 0)
	{
		*ret = true;
		return (false);
	}
	if (io->heredoc_quotes == false && ft_strchr(*line, '$'))
	{
		*line = var_line_expand(data, *line);
		if (!(*line))
		{
			free_ptr(*line);
			*ret = false;
			return (false);
		}
	}
	return (true);
}

bool	fill_heredoc(t_data *data, t_inout_fds *io, int fd)
{
	char	*line;
	bool	ret;

	ret = false;
	line = NULL;
	while (1)
	{
		setup_signals(); //interactive
		line = readline(">");
		setup_signals(); // non-interactive
		if (!eval_heredoc(data, &line, io, &ret))
			break ;
		ft_putendl_fd(line, fd); //to add
		free_ptr(line);
	}
	free_ptr(line);
	return (ret);
}