NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFTPATH = libft
LIBFT = $(LIBFTPATH)/libft.a

PRINTFPATH = printf
PRINTF = $(PRINTFPATH)/ft_printf.a

SRCS = check_sign.c main.c heredoc.c get_next_line/get_next_line.c signals.c\
		get_next_line/get_next_line_utils.c set_types.c syntax_erros.c set_list.c\
		env.c built_in_cmds.c create_tree.c parsing_exit.c export_utils.c\
		execute_cmd.c execute_cmd_utils.c execute.c execute_pipe.c expansion.c\
		execute_builtin.c cd.c echo.c free.c create_tree_utils.c export_unset.c \
		redirections.c brackets.c shell_loop.c split_nodes.c wildcard_utils.c \
		wildcard.c set_list_utils.c execute_cmd_utils2.c heredoc_utils.c
OBJS = $(SRCS:.c=.o)

HEADERS = minishell.h

RM = rm -f

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTPATH)

$(PRINTF):
	$(MAKE) -C $(PRINTFPATH)

$(NAME): $(OBJS) $(LIBFT) $(PRINTF)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(PRINTF) -lreadline -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFTPATH) clean
	$(MAKE) -C $(PRINTFPATH) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTPATH) fclean
	$(MAKE) -C $(PRINTFPATH) fclean

re: fclean all

.PHONY: all clean fclean re
