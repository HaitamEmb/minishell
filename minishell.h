#ifndef MINISHELL_H
# define MINISHELL_H
# define FAILURE 0
# define SUCCESS 1

/*-------------Includes--------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/*-------------ENUMS------------------*/

enum e_token_type
{
	SPACE = 1,
	WORD,
	VAR,
	PIPE,
	OUTPUT,
	HEREDOC,
	APPEND,
	INPUT,
	END
};

enum e_token_quote
{
	DEFAULT,
	SINGLE,
	DOUBLE
};

/*-------------Structs---------------*/

typedef struct s_inout_fds
{
	char	*infile;
	char	*outfile;
	char	*heredoc_del;
	bool	heredoc_quotes;
	int	fd_in;
	int	fd_out;
	int	stdin_backup;
	int	stdout_backup;
}	t_inout_fds;

typedef struct s_token
{
	char	*str;
	char	*str_back;
	bool	is_var_exist;
	int	status;
	int	type;
	bool	to_join;
	struct	s_token *prev;
	struct	s_token	*next;
}	t_token;

typedef	struct s_command
{
	char	*command;
	char	**args;
	char	*path;
	bool	pipe_out;
	int	*pipe_fd;
	t_inout_fds	*inout_fds;
	struct	s_command	*next;
	struct	s_command	*prev;
}	t_command;

/*---------------add other struct---------*/
typedef	struct	s_data
{
	bool	interactive_mode;
	t_token	*token;
	char	*user_input;
	char	**env;
	char	*work_dir;
	char	*old_work_dir;
	t_command	*cmd;

}	t_data;

/*-------------------FUNCS----------------*/


int	create_token(char *str, t_data *data);

int	is_var(t_token **lst_token);
int	is_invalid_ops(t_token **token);

int	set_status(int status, char *str, int i);
int	word_or_command(int *i, char *str, int start, t_data *data);
int	is_command(char *str, int i);

t_token *lst_insert_between(t_token **head, t_token *to_del, t_token *insert);
t_token *lst_new_token(char *str, char *str_back, int type, int status);
void	lst_add_prev(t_token **head, t_token *new_token);
void	lst_deltoken(t_token *token, void (*del)(void *));
void	lst_clear(t_token **head, void (*del)(void *));
//to add for lexer; handle input : space and put the input into the structure to give for execution.

			/*EXPAND*/

int	expand_variables(t_data *data, t_token **token);
char	*expand_var_heredoc(t_data *data, char *str);
char	*recover_val(t_token *token, char *str, t_data *data);
char	*var_id(char *str);
int	var_len(char *str);
bool	is_var_valid(char c);
int	replace_var(t_token **token, char *var_value, int index);
char	*replace_herdoc_var(char *str, char *var_val, int index);
char	create_token_string(char *old_str, char *var_val, int n_size, int index);
void	copy_to_new_str(char *n_str, char *var_val, int *j);

			/*HANDLE QUOTES*/
bool	str_has_quotes(char *str);
int	handle_quotes(t_data *data);
int	quote_str_len(char *str, int count, int i);
int	remove_quotes(t_token **token);

			/*PARSING*/

void	parse_word(t_command **cmd, t_token **token);
int	fill_args(t_token **token, t_command *last_cmd);
int	add_args_default(t_token **token, t_command *last_cmd);
int	add_args_echo(t_token **token, t_command *last_cmd);
int	create_args_default(t_token **token, t_command *last_cmd);
int	create_args_echo(t_token **token, t_command *last_cmd);
int	args_len(t_token *tmp);
int	args_len_echo(t_token *tmp);
void	create_cmd(t_data *data, t_token *token);
char	**copy_in_new_tab(int len, char **n_tab, t_command *last_cmd, t_token *tmp);
void	del_empty_args(t_token **token);
char	*join_vars(t_token **token);

#endif