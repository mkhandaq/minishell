/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialausud <ialausud@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:36:47 by ialausud          #+#    #+#             */
/*   Updated: 2026/02/14 19:34:05 by ialausud         ###   ########.fr       */
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
            && tmp->next && tmp->next->type == TOK_LIMITER)
        {
            limiter = tmp->next->value;
            filename = gen_heredoc_name();
            if (!filename)
            {
                perror("heredoc filename");
                return;
            }
            fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("heredoc open");
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
            tmp->type = TOK_REDIR_IN;
            tmp->next->type = TOK_INFILE;
            free(tmp->next->value);
            tmp->next->value = filename;
        }
        tmp = tmp->next;
    }
}
