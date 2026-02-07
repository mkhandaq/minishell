NAME = ft_printf.a

SRC	=	ft_printf.c	\
		ft_putchar.c	\
		ft_puthexl.c	\
		ft_puthexu.c	\
		ft_putnbr.c	\
		ft_putptr.c	\
		ft_putstr.c	\
		ft_putun.c

OBJS	=	$(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -f

all: $(NAME)
$(NAME):	$(OBJS)
		ar -rcs $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY:			all clean fclean re
