NAME		= minishell
CC		= cc
CFLAGS		= -Wall -Wextra -Werror -I.
LDFLAGS	= -lreadline -lncurses
RM		= rm -f

SRCS		= \
	main.c \
	parsing/create_cmd.c \
	parsing/create_token.c \
	parsing/expand_var.c \
	parsing/expand_var_utils.c \
	parsing/fill_cmd_args.c \
	parsing/fill_echo_args.c \
	parsing/fill_utils.c \
	parsing/get_value.c \
	parsing/handle_quotes.c \
	parsing/is_invalid_ops.c \
	parsing/is_var.c \
	parsing/parsing_cmd.c \
	parsing/remove_quotes.c \
	parsing/replace_vars.c \
	parsing/token_lst.c \
	parsing/token_lst2.c \
	parsing/token_utils.c \
	parsing/var_id.c \
	parsing/cmd_lst_utils.c \
	parsing/cmd_lst_utils2.c \
	parsing/parse_pipe.c \
	parsing/parse_input.c \
	parsing/parse_trunc.c \
	parsing/parse_append.c \
	parsing/parse_heredoc.c \
	parsing/fill_heredoc.c \
	execution/builtins_basic.c \
	execution/builtin_cd.c \
	execution/builtin_export.c \
	execution/builtin_exit.c \
	execution/builtins.c \
	execution/env_utils.c \
	execution/env_utils2.c \
	execution/export_utils.c \
	execution/executor.c \
	execution/executor_child.c \
	execution/executor_utils.c \
	execution/executor_utils2.c \
	execution/heredoc.c \
	execution/path_utils.c \
	execution/redirections.c \
	execution/signals.c \
	utils/ft_calloc.c \
	utils/ft_is.c \
	utils/ft_itoa.c \
	utils/ft_split.c \
	utils/ft_strchr.c \
	utils/ft_strcmp.c \
	utils/ft_strjoin.c \
	utils/ft_strlen.c \
	utils/ft_strncmp.c \
	utils/ft_substr.c \
	utils/ft_strdup.c \
	utils/ft_strtrim.c \
	utils/free_utils.c \
	utils/error_utils.c \
	utils/ft_putendl_fd.c

OBJS		= $(SRCS:.c=.o)

all:	$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
