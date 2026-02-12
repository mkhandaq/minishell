/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhandaq <mkhandaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:36:47 by ialausud          #+#    #+#             */
/*   Updated: 2026/02/11 19:22:35 by mkhandaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

// This function generates a unique filename for the heredoc temporary file. It uses a static variable to keep track of the index and concatenates it with a prefix to create the filename.
static char *gen_heredoc_name(void)
{
    static int  idx = 0;
    char        *num;
    char        *name;
    char        *prefix = "/tmp/shellGuys_heredoc_";

    num = ft_itoa(idx++);
    if (!num)
        return (NULL);
    name = ft_strjoin(prefix, num);
    free(num);
    return (name);
}
// This function processes the heredoc tokens in the list. 
// It iterates through the list and when it finds a TOK_HEREDOC token followed by a TOK_INFILE token,
// it reads lines from the user until the limiter is reached and writes them to a temporary file.
// It then updates the TOK_HEREDOC token to TOK_REDIR_IN and sets the value of the next token to the name of the temporary file.
void    process_heredocs(t_token *list)
{
    t_token *tmp = list;
    char    *limiter;
    char    *filename;
    int     fd;
    char    *line;

    while (tmp)
    {
        if (tmp->type == TOK_HEREDOC
            && tmp->next && tmp->next->type == TOK_OUTFILE)
        {
            limiter = tmp->next->value;
            filename = gen_heredoc_name();
            if (!filename)
                return;

            fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd == -1)
            {
                free(filename);
                return;
            }

            while (1)
            {
                line = readline("> ");
                if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
                {
                    free(line);
                    break;
                }
                write(fd, line, ft_strlen(line));
                write(fd, "\n", 1);
                free(line);
            }
            close(fd);

            //tmp->type = TOK_REDIR_IN; // we change the type of the current node to TOK_REDIR_IN because we will treat it as a regular input redirection in the execution phase
                                        // we set the value of the next node to the name of the temporary file
            free(tmp->next->value);
            tmp->next->value = filename;
        }
        tmp = tmp->next;
    }
    fd = open (filename, O_RDONLY);
    line = get_next_line(fd);
    while (line)
    {
        ft_printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
}
