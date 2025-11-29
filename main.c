/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:33:00 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:33:00 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void init_data(t_data *data, char **envp)
{
	data->interactive_mode = true;
	data->token = NULL;
	data->user_input = NULL;
	data->env = dup_env(envp);
	data->work_dir = getcwd(NULL, 0);
	data->old_work_dir = ft_strdup(data->work_dir);
	data->cmd = NULL;
	if (!data->env)
		perror("env");
}

static void reset_state(t_data *data)
{
	if (data->token)
		lst_clear(&data->token, free_ptr);
	if (data->cmd)
		clear_cmd_list(&data->cmd);
	free_ptr(data->user_input);
	data->user_input = NULL;
}

static void cleanup_data(t_data *data)
{
	reset_state(data);
	free_str_tab(data->env);
	free_ptr(data->work_dir);
	free_ptr(data->old_work_dir);
}

static void run_line(t_data *data)
{
	if (!data->user_input || data->user_input[0] == '\0')
		return;
	if (create_token(data->user_input, data) != SUCCESS)
		return;
	if (handle_quotes(data) != SUCCESS)
		return;
	if (expand_variables(data, &data->token) != SUCCESS)
		return;
	remove_quotes(&data->token);
	create_cmd(data, data->token);
	if (data->cmd)
		run_execution(data);
}

int main(int argc, char **argv, char **envp)
{
	t_data data;

	(void)argc;
	(void)argv;
	init_data(&data, envp);
	setup_signals();
	while (1)
	{
		data.user_input = readline("minishell$ ");
		if (!data.user_input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (data.user_input[0])
			add_history(data.user_input);
		run_line(&data);
		reset_state(&data);
	}
	cleanup_data(&data);
	return (g_exit_status);
}
