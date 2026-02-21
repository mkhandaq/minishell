#ifndef MINISHELL_H
#define MINISHELL_H

# define GREEN  "\001\033[32m\002"
# define BLUE   "\001\033[34m\002"
# define RESET  "\001\033[0m\002"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include "printf/printf.h"
#include "get_next_line.h"

extern int global_signal;

typedef enum e_toktype {
	TOK_CMD,
	TOK_KEYWORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC,
	TOK_INFILE,
	TOK_OUTFILE,
	TOK_LIMITER,
	TOK_OPENBRC,
	TOK_CLOSEBRC
} t_toktype;

typedef struct s_token {
	t_toktype		type; 
	int				is_exuted;   
    char			*value;
    struct s_token	*next;
}	t_token;

t_token	*set_list(char *input);

void	set_types(t_token **list);
void    process_heredocs(t_token *list);
void    set_signals(void);
void    env(char **str);
void    pwd(char **str);

char 	**export(char **env, char *added_var);

int 	set_sign(t_token **node);
int		is_two_sided(t_token *node);
int		is_sign(t_token *node);
int		is_one_sided(t_token *node);
int		check_syntax_errors(t_token *node);
int		return_sign_len(t_token *node);

#endif