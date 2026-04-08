*This project has been created as part of the 42 curriculum by mkhandaq, aalemami.*

# minishell

## Description

This project is a custom Unix-like shell written in C as part of the 42 curriculum.

Its goal is to reproduce the core behavior of a real shell while learning low-level system programming concepts such as process creation, signal handling, file descriptor manipulation, parsing, and environment management.

At a high level, this minishell:

- reads commands in interactive mode (Readline) and non-interactive mode (stdin stream),
- tokenizes input with quote awareness,
- validates syntax (including operators and brackets),
- builds an execution tree (AST) using operator precedence,
- performs expansions (environment variables, `$?`, wildcard `*`),
- handles redirections (`<`, `>`, `>>`, `<<`),
- executes built-ins and external binaries,
- propagates exit status across pipelines, logical operators, and subshells.

### Implemented Features

- Built-ins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Operators: `|`, `&&`, `||`
- Grouping with parentheses for subshell execution
- Redirections: input, output, append, heredoc
- Environment expansion: `$VAR` and `$?`
- Wildcard expansion for `*` patterns in the current directory
- Signal behavior for interactive shell and heredoc interruption

### Notes

- In non-interactive mode, the shell prints `exit` when stdin reaches EOF.
- Wildcard support is focused on `*` matching.

## Instructions

### Prerequisites

- A C compiler (`cc`/`gcc`)
- `make`
- Readline development library (for example, `libreadline-dev` on Debian/Ubuntu)

### Build

```bash
make
```

### Run

Interactive:

```bash
./minishell
```

Non-interactive examples:

```bash
echo "pwd" | ./minishell
printf "echo hello\nexit\n" | ./minishell
```

### Clean Artifacts

```bash
make clean
make fclean
make re
```

## Resources

Classic references related to shell implementation and Unix process control:

- 42 minishell subject and project guidelines (intra)
- GNU Bash Reference Manual: https://www.gnu.org/software/bash/manual/
- POSIX Shell Command Language: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
- Linux man pages: `fork(2)`, `execve(2)`, `waitpid(2)`, `pipe(2)`, `dup2(2)`, `open(2)`, `signal(2)`, `readline(3)`

### AI Usage

AI was used as a support tool for:

- Helping creating this README file.
- Explaining some of the shell concepts.

All implementation decisions, code integration, and final validation were performed manually in the project codebase.