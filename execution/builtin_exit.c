#include "minishell.h"

static int is_numeric(const char *str)
{
	int i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isnum(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static long long to_long_long(const char *str, int *overflow)
{
	unsigned long long limit;
	unsigned long long value;
	int sign;
	int digit;

	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	limit = (sign == 1) ? (unsigned long long)LLONG_MAX
						: (unsigned long long)LLONG_MAX + 1ULL;
	value = 0;
	while (*str)
	{
		digit = *str - '0';
		if (value > (limit - digit) / 10)
		{
			*overflow = 1;
			return (0);
		}
		value = value * 10 + digit;
		str++;
	}
	if (sign == -1)
	{
		if (value == limit)
			return (LLONG_MIN);
		if (value > limit)
		{
			*overflow = 1;
			return (0);
		}
		return ((long long)(-value));
	}
	return ((long long)value);
}

int builtin_exit(t_data *data, t_command *cmd, bool in_child)
{
	int overflow;
	long long value;

	(void)data;
	if (!cmd || !cmd->args)
		return (0);
	if (!in_child)
		write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd->args[1])
		exit(g_exit_status);
	if (!is_numeric(cmd->args[1]))
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": exit: ", sizeof(": exit: ") - 1);
		write(STDERR_FILENO, cmd->args[1], ft_strlen(cmd->args[1]));
		write(STDERR_FILENO, ": numeric argument required\n",
			  sizeof(": numeric argument required\n") - 1);
		if (in_child)
			exit(255);
		g_exit_status = 255;
		return (g_exit_status);
	}
	if (cmd->args[2])
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": exit: too many arguments\n",
			  sizeof(": exit: too many arguments\n") - 1);
		g_exit_status = 1;
		return (g_exit_status);
	}
	overflow = 0;
	value = to_long_long(cmd->args[1], &overflow);
	if (overflow)
	{
		write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
		write(STDERR_FILENO, ": exit: numeric argument required\n",
			  sizeof(": exit: numeric argument required\n") - 1);
		if (in_child)
			exit(255);
		g_exit_status = 255;
		return (g_exit_status);
	}
	g_exit_status = (int)(value & 0xFF);
	if (in_child)
		exit((unsigned char)g_exit_status);
	exit((unsigned char)g_exit_status);
	return (g_exit_status);
}
