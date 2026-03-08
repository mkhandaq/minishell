#ifndef MINISHELL_H
#define MINISHELL_H

# define GREEN  "\001\033[32m\002"
# define BLUE   "\001\033[34m\002"
# define RESET  "\001\033[0m\002"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
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
	TOK_CLOSEBRC,
	TOK_ECHO,
	TOK_CD,
	TOK_PWD,
	TOK_EXPORT,
	TOK_UNSET,
	TOK_ENV,
	TOK_EXIT,
	TOK_SUBSHELL
} t_toktype;

typedef enum e_strtype {
	TOK_STR,
	TOK_SING,
	TOK_DUP
}	t_strtype;

typedef struct s_token {
	t_toktype		type; 
	t_strtype		strtype;
	int				is_exuted;   
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_tree {
	t_toktype		type;
	t_token			*tokens;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;


t_token	*set_list(char *input);
t_tree	*build_tree(t_token *tokens);

int		set_types(t_token **list);
void    process_heredocs(t_token *list);
void    set_signals(void);
int	env(char **envt);
int	pwd(void);
char	*ft_getenv(char **env, const char *name);
void	set_built_in_cmds(t_token **node);
int	echo(t_token **list);
void	free_list(t_token	**list);
void	print_tree(t_tree *tree);
void	free_tree(t_tree *tree);
void	print_export(char **env);
void    free_2d (char **args);
void	expand_tokens(t_token *list, int exit_status, char **env);

char	*get_path(char *cmd, char **ev);

char	**set_whole_command(t_token *list, char *cmd);
char 	**export(char **env, char *added_var);

int		is_valid_identifier(const char *str);
int		builtin_exit(t_token *node); //new
int 	set_sign(t_token **node);
int		is_two_sided(t_token *node);
int		is_sign(t_token *node);
int		is_one_sided(t_token *node);
int		is_built_in(t_token *node);
int		check_syntax_errors(t_token *node);
int		return_sign_len(t_token *node);
int		is_file(t_token *node);
int		cd(char **args, char ***env);
int		redirections(t_token *list);
int		execute_cmd(char ***env, t_token *list, int *last_exit);
int		execute(t_tree *tree, char ***env, int *last_exit);
int		execute_pipe(t_tree *tree, char ***env, int *last_exit);
int		execute_builtin(t_token *list, char ***envp, int *last_exit);

char	**unset(char **env, char *removed_var);


#endif