NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror 

LIBFTPATH = libft
LIBFT = $(LIBFTPATH)/libft.a

PRINTFPATH = printf
PRINTF = $(PRINTFPATH)/ft_printf.a

SRCS = check_sign.c main.c set_list.c heredoc.c get_next_line.c get_next_line_utils.c set_types.c syntax_erros.c
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