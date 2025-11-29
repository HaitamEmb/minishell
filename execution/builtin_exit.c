/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:27:01 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:27:01 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_overflow(unsigned long long value, unsigned long long limit,
		int digit, int *overflow)
{
	if (value > (limit - digit) / 10)
	{
		*overflow = 1;
		return (1);
	}
	return (0);
}

static long long	to_long_long(const char *str, int *overflow)
{
	unsigned long long	limit;
	unsigned long long	value;
	int					sign;

	sign = 1;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	limit = (unsigned long long)LLONG_MAX;
	if (sign == -1)
		limit = (unsigned long long)LLONG_MAX + 1ULL;
	value = 0;
	while (*str)
	{
		if (check_overflow(value, limit, *str - '0', overflow))
			return (0);
		value = value * 10 + (*str++ - '0');
	}
	if (sign == -1 && value > limit)
		*overflow = 1;
	if (sign == -1)
		return ((long long)(-value));
	return ((long long)value);
}

static int	exit_error(char *arg, bool in_child)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": exit: ", sizeof(": exit: ") - 1);
	if (arg)
		write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n",
		sizeof(": numeric argument required\n") - 1);
	if (in_child)
		exit(255);
	g_exit_status = 255;
	return (g_exit_status);
}

static int	exit_too_many_args(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": exit: too many arguments\n",
		sizeof(": exit: too many arguments\n") - 1);
	g_exit_status = 1;
	return (g_exit_status);
}

int	builtin_exit(t_data *data, t_command *cmd, bool in_child)
{
	int			overflow;
	long long	value;

	(void)data;
	if (!cmd || !cmd->args)
		return (0);
	if (!in_child)
		write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd->args[1])
		exit(g_exit_status);
	if (!is_numeric_str(cmd->args[1]))
		return (exit_error(cmd->args[1], in_child));
	if (cmd->args[2])
		return (exit_too_many_args());
	overflow = 0;
	value = to_long_long(cmd->args[1], &overflow);
	if (overflow)
		return (exit_error(NULL, in_child));
	g_exit_status = (int)(value & 0xFF);
	exit((unsigned char)g_exit_status);
	return (g_exit_status);
}
