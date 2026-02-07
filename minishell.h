#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "lib/libft.h"
#include "printf/printf.h"

typedef enum e_toktype {
	TOK_CMD,
	TOK_KEYWORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC
} t_toktype;

typedef struct s_token {
	t_toktype		type;    
    char			*value;
    struct s_token	*next;
}	t_token;

void	set_list(t_token **list, char **argv);
void 	set_sign(t_token **node);
void	set_types(t_token **list);

int		is_two_sided(t_token *node);
int		is_sign(t_token *node);
int		is_one_sided(t_token *node);
int		check_syntax_errors(t_token *node);

#endif