# Applied Changes: Explanation (Before and After)

This document explains the current code changes in:
- execute_cmd.c
- execute_cmd_utils2.c
- heredoc.c
- set_list_utils.c
- shell_loop.c

## 1) execute_cmd.c

### Why this change was made
The shell previously skipped redirection-only inputs when no command token existed.

Example of affected behavior:
- `> /tmp/ms_test_file`
- `<<l <<ls>0`

Before this fix, these could parse but not execute redirections in the no-command path.

### Before
```c
*cmd = find_command(list);
if (!*cmd)
{
    *last_exit = 0;
    return (0);
}
```

### After
```c
static int process_redirections_only(t_token *list)
{
    int stdin_copy;
    int stdout_copy;
    int status;

    stdin_copy = dup(STDIN_FILENO);
    stdout_copy = dup(STDOUT_FILENO);
    if (stdin_copy == -1 || stdout_copy == -1)
    {
        if (stdin_copy != -1)
            close(stdin_copy);
        if (stdout_copy != -1)
            close(stdout_copy);
        return (1);
    }
    status = 0;
    if (!redirections(list))
        status = 1;
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdin_copy);
    close(stdout_copy);
    return (status);
}

*cmd = find_command(list);
if (!*cmd)
{
    *last_exit = process_redirections_only(list);
    return (0);
}

tmp = list;
while (tmp)
{
    if (is_built_in(tmp))
    {
        *last_exit = process_built_in(env, list, last_exit, *cmd);
        return (0);
    }
    tmp = tmp->next;
}
```

### Before behavior
No command => immediate return, redirections not applied.

### After behavior
No command => redirections are applied safely, stdio is restored, status reflects success/failure.

### Norminette note
To pass norminette (`TOO_MANY_FUNCS`), one helper (`has_builtin_cmd`) was removed and its loop was inlined inside `setup_execute_cmd`. This is a style-only refactor and does not change behavior.

---

## 2) execute_cmd_utils2.c

### Why this change was made
`Ctrl+\\` on external commands like `sleep 100` was incorrect because child SIGQUIT handling was not restored to default.

### Before
```c
if (pid == 0)
{
    signal(SIGINT, SIG_DFL);
    exec_child(list, whole_cmd, *env, path);
}
...
if (WIFSIGNALED(status))
{
    g_signal = WTERMSIG(status);
    write(1, "\n", 1);
    return (128 + WTERMSIG(status));
}
```

### After
```c
if (pid == 0)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    exec_child(list, whole_cmd, *env, path);
}
...
if (WIFSIGNALED(status))
{
    g_signal = WTERMSIG(status);
    if (g_signal == SIGINT)
        write(1, "\n", 1);
    else if (g_signal == SIGQUIT)
        write(2, "Quit: 3\n", 8);
    return (128 + g_signal);
}
```

### Before behavior
`Ctrl+\\` handling was inconsistent for external command execution.

### After behavior
External child receives SIGQUIT normally; shell reports `Quit: 3` and returns correct signal-based status.

---

## 3) heredoc.c

### Why this change was made
`Ctrl+C` during heredoc should not print EOF warning text.

### Before
```c
ft_putstr_fd("shellGuys: warning: ", 2);
ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
ft_putstr_fd(tmp->next->value, 2);
ft_putstr_fd("')\n", 2);
if (g_signal == SIGINT)
    return (1);
return (0);
```

### After
```c
if (g_signal == SIGINT)
    return (1);
ft_putstr_fd("shellGuys: warning: ", 2);
ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
ft_putstr_fd(tmp->next->value, 2);
ft_putstr_fd("')\n", 2);
return (0);
```

### Before behavior
Interrupted heredoc could still emit EOF warning.

### After behavior
Interrupted heredoc exits cleanly without misleading warning.

---

## 4) set_list_utils.c

### Why this change was made
Tokenizer was whitespace-driven only, so operators adjacent to quoted or plain text were merged into one token.

Affected examples:
- `echo "something"|cat`
- `echo "hi"||ls`

### Before
```c
while (input[*i] && input[*i] != ' ' && input[*i] != '\t')
{
    if (input[*i] == '\'' || input[*i] == '"')
    {
        ...
        continue ;
    }
    (*i)++;
}
```

### After
```c
static void read_word_token(char *input, int *i)
{
    ...
}

if (ft_strchr("|&<>()", input[*i]))
{
    if ((input[*i] == '|' && input[*i + 1] == '|')
        || (input[*i] == '&' && input[*i + 1] == '&')
        || (input[*i] == '<' && input[*i + 1] == '<')
        || (input[*i] == '>' && input[*i + 1] == '>'))
        (*i) += 2;
    else
        (*i)++;
    *stype = TOK_STR;
    return (ft_substr(input, start, *i - start));
}

while (input[*i] && input[*i] != ' ' && input[*i] != '\t')
{
    if (input[*i] == '\'' || input[*i] == '"')
    {
        ...
        continue ;
    }
    if (ft_strchr("|&<>()", input[*i]))
        break ;
    (*i)++;
}

*stype = TOK_STR;
if (input[start] == '\'')
    *stype = TOK_SING;
if (input[start] == '"')
    *stype = TOK_DUP;
```

### Before behavior
`|`, `||`, `<<`, etc. could become part of adjacent tokens.

### After behavior
Operators are tokenized correctly even without surrounding spaces.

### Norminette note
Two style-only adjustments were made after the functional fix:
- A helper (`read_word_token`) was introduced so `read_token` stays within line limits.
- A ternary expression for `stype` was replaced by `if` statements (norminette forbids ternaries).
These do not change parser behavior.

---

## 5) shell_loop.c

### Why this change was made
`Ctrl+C` on empty prompt should set shell exit status to 130.

### Before
```c
while (1)
{
    input = shell_read(last_exit);
    node = set_list(input);
    ...
}
```

### After
```c
while (1)
{
    input = shell_read(last_exit);
    if (g_signal == SIGINT)
    {
        last_exit = 130;
        g_signal = 0;
    }
    node = set_list(input);
    ...
}
```

### Before behavior
Prompt interruption did not consistently update `$?` to 130.

### After behavior
After empty-prompt `Ctrl+C`, `$?` is 130.

---

## Quick Validation Summary

Validated after changes:
- `echo "something"|cat` -> correct pipeline behavior.
- `echo "hi"||ls` -> correct logical OR tokenization.
- `sleep 100` then `Ctrl+\\` -> `Quit: 3` and correct signal status handling.
- heredoc + `Ctrl+C` -> no incorrect EOF warning.
- empty prompt + `Ctrl+C` -> `echo $?` returns `130`.
- redirection-only input (`> /tmp/ms_test_file`) -> redirection executes.

## Final Status (Norminette)

Norminette status for modified C files:
- `execute_cmd.c`: OK
- `execute_cmd_utils2.c`: OK
- `heredoc.c`: OK
- `set_list_utils.c`: OK
- `shell_loop.c`: OK
