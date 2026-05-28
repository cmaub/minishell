
<div align="center">


# Minishell

**A minimal Unix shell implementation written in C** 

 ![Language](https://img.shields.io/badge/language-C-blue)
 ![Score](score.png)

</div>

---

## Overview

`minishell` reproduces the core execution model of a POSIX-compliant shell — command parsing, pipeline orchestration, I/O redirection, environment management, and signal handling — using only C system calls and the readline library.

The project is a practical study of how a shell interacts with the Unix kernel: process creation via `fork`/`execve`, inter-process communication through pipes, file descriptor lifecycle management, and the terminal signal model. Every component was implemented from scratch without relying on `system()` or shell invocation.

--- 

## Table of Contents

1. [Architecture](#architecture)
2. [Execution Pipeline](#execution-pipeline)
3. [Security & Robustness](#security--robustness)
4. [Builtins](#builtins)
5. [Signal Handling](#signal-handling)
6. [Build & Usage](#build--usage)
7. [Examples](#examples)
8. [Known Limitations](#known-limitations)
9. [References](#references)

---

## Architecture

The shell is structured as a linear pipeline of subsystems. Each stage transforms its input into a well-defined data structure consumed by the next.

```
┌─────────────────────────────────────────────────────────────────┐
│                        MAIN LOOP                                │
│  readline() → signal setup → tokenize → parse → exec → reset    │
└─────────────────────────────────────────────────────────────────┘

  Raw input string (char *)
       │
       ▼
  ┌─────────┐    t_token linked list
  │  LEXER  │ ──────────────────────►  { value, type, prev, next }
  └─────────┘    type: ARG | REDIRECT_IN | REDIRECT_OUT |
                       APPEND_OUT | HEREDOC | PIPEX
       │
       ▼
  ┌──────────┐   t_parser node list
  │  PARSER  │ ──────────────────────►  { command[], file[], redir[],
  └──────────┘                           delimiter[], fd_heredoc[][] }
       │
       ▼
  ┌──────────────┐  resolved strings (in-place)
  │   EXPANDER   │ ─────────────────────────────►  $VAR → value
  └──────────────┘                                 $? → exit code
                                                   quote stripping
       │
       ▼
  ┌──────────────┐
  │   EXECUTOR   │
  └──────────────┘
       │
       ├── builtin? ──► exec in parent (or subshell in pipeline)
       │
       └── external? ─► fork() → child: setup fds, execve()
                                  parent: waitpid(), collect status
```

### Key Data Structures

| Structure | Role |
|-----------|------|
| `t_lexer` | Cursor over the raw input string (`data`, `head`, `len`)
| `t_token` | Doubly-linked list of classified tokens
| `t_parser` | Node representing one command with its redirections, files, and heredoc fds
| `t_pipex` | Executor state: pipe fds, pid tracking, env array, exit code
| `t_env` | Doubly-linked list representing the environment (owns all strings)
| `t_mega` | Top-level context passed through all stages; owns all subsystem pointers


## Execution Pipeline


### 1. Lexer

The lexer operates as a recursive descent scanner over a `t_lexer` cursor. It classifies each token using character-level predicates (`ft_digit`, `dquote`, `squote`, `ft_dollar`, etc.) and emits typed `t_token` nodes.

Special handling:
- Single quotes: content is never expanded; the token is flagged accordingly.
- Double quotes: content is subject to `$VAR` expansion but not word-splitting.
- `<<` (heredoc): the delimiter is captured at lex time; the heredoc pipe is opened immediately to avoid ordering issues at execution.

### 2. Parser

The parser walks the token list and groups tokens into `t_parser` nodes, one per command segment (pipe-separated). Each node accumulates:
- `command[]` — argv-style array for `execve`
- `file[]` + `redir[]` — paired arrays of filenames and their redirection types
- `fd_heredoc[][]` — pre-opened pipe fds for each heredoc in this command

Design decision: heredocs are fully read and written to a pipe during parsing, before any child process exists. This avoids a race condition between the heredoc reader and the executor, and correctly handles SIGINT interruption (the pipe is simply closed and the exit code set to 130).

### 3. Expander

Expansion runs over every string in the node (command tokens and redirection filenames) after parsing but before execution. It handles:

- `$VARNAME` → value looked up in the `t_env` linked list
- `$?` → current exit code (`mini->exit_code`)
- `$` followed by nothing, a digit, or a special char → left as-is
- Quote removal: single-quoted regions are stripped without expansion; double-quoted regions are stripped after expansion.

The expander processes strings character-by-character via `process_string()`, dispatching to `withdraw_quotes()`, `withdraw_double()`, or `expand_var()` depending on context.

### 4. Executor

For a single command (no pipe), execution runs in the parent process if the command is a builtin; otherwise it forks. For pipelines:

```
parent
  │
  ├─ pipe(pipefd)
  ├─ fork() → child[0]: stdin=prev_fd, stdout=pipefd[1], exec cmd[0]
  ├─ pipe(pipefd)
  ├─ fork() → child[1]: stdin=prev_fd, stdout=pipefd[1], exec cmd[1]
  │   ...
  └─ fork() → child[n]: stdin=prev_fd, stdout=original stdout, exec cmd[n]
       waitpid(last_pid) → collect exit status of last command only
```

The exit code of a pipeline is the exit status of the last command, matching bash behavior.

---

## Security & Robustness

### File Descriptor Management

Every file descriptor opened during execution (pipes, heredoc pipes, redirected files) is tracked and closed at the correct point in the child process lifecycle:

- In the parent: `pipefd[1]` (write end) is closed after `fork()`, preventing the child from blocking on an open write end.
- In the child: all pipe fds not needed for this specific command are closed before `execve()`. A failure to close these would leak fds into the executed process and potentially cause hangs in pipelines.
- On error paths: `close_error_and_free()` is called uniformly — there is no error path that skips fd cleanup.

### Heredoc & SIGINT

Heredoc input is read in the parent process before forking. If the user presses `Ctrl-C` during heredoc input:
1. The `SIGINT` handler sets `g_signal = 1` and writes a newline to maintain terminal state.
2. The heredoc reading loop detects `g_signal` and breaks.
3. The heredoc pipe is closed and the exit code is set to 130.
4. No zombie process is left; no orphan pipe fd remains open.

This matches the behavior of bash: `Ctrl-C` during a heredoc cancels the heredoc and returns to the prompt with exit code 130.

### Signal Isolation Between Parent and Child

Three distinct signal contexts are handled:

| Context | SIGINT | SIGQUIT |
|--------|--------|---------|
| Interactive shell (parent) | `handle_c_signal` — rewrites prompt | `SIG_IGN` |
| Child process (external cmd) | `SIG_DFL` — default kernel behavior | `SIG_DFL` |
| Heredoc input | `handle_c_signal_heredoc` — sets flag, breaks read loop | `SIG_IGN` |

Before `execve()`, the child resets both signals to `SIG_DFL`. This prevents a child from inheriting a handler that references parent-only state (e.g., readline internals).

### Input Sanitisation & Variable Expansion

The expander treats all user input as untrusted:
- Variable names are validated character-by-character before lookup; invalid identifiers are left unexpanded rather than causing an error.
- Empty variable expansion (`$UNSET_VAR`) produces an empty string and is subject to word-splitting rules (i.e., removed from the argument list unless quoted).
- `$?` is always resolved from the shell's tracked exit code, never from the process environment.

### Memory Ownership

Each subsystem has a clearly defined ownership model:
- `t_env` owns all its strings; `ft_unset` and `ft_export` free/replace strings in-place.
- `t_parser` nodes own their `command[]` and `file[]` arrays; `free_new_node()` is the single free point.
- `t_mega` is the root owner; `free_exec_input()` releases the parser list and token list after each command cycle, without touching the environment or the main `t_mega` struct itself.
- No string is duplicated into a node without being owned by that node. Functions that take strings by reference are documented not to store them.

---

## Builtins

Builtins execute in the parent process (except when inside a pipeline, where they run in a subshell to avoid polluting the parent's state).

| Builtin | Behavior notes |
|---------|----------------|
| `echo` | Supports `-n` flag; no other options. |
| `cd` | Updates `PWD` and `OLDPWD` in the env list. Supports `-` (previous dir). |
| `pwd` | Reads from `getcwd()`, not from the `PWD` variable. |
| `export` | Validates variable names (must start with alpha or `_`, no `=` in name). Prints sorted env if no argument. |
| `unset` | Removes variable from the env linked list; no-op if the variable does not exist. |
| `env` | Prints the current environment without any sorting. |
| `exit` | Validates the argument as a numeric string; exits with `arg % 256`. Prints an error and sets exit code 2 on invalid argument. |

---

## Signal Handling

Signal behavior is configured at three points in the lifecycle:

```
main loop start:
  SIGINT  → handle_c_signal        (rewrite prompt via rl_replace_line)
  SIGQUIT → SIG_IGN

before fork (child process entry):
  SIGINT  → SIG_DFL
  SIGQUIT → SIG_DFL

heredoc read loop:
  SIGINT  → handle_c_signal_heredoc (set g_signal flag, break loop)
  SIGQUIT → SIG_IGN
```

`g_signal` is the only global variable. It is a `volatile sig_atomic_t`-compatible `int`, set exclusively in signal handlers and read in the main loop to detect interruption. It is reset to 0 at the start of each readline cycle.

---

## Build & Usage

### Dependencies

- `gcc` or `clang`
- `libreadline` (`libreadline-dev` on Debian/Ubuntu)
- `libft` (included as a submodule / local library)

### Build

```bash
make          # build minishell
make clean    # remove object files
make fclean   # remove object files and binary
make re       # full rebuild
```

### Run

```bash
./minishell
```

The shell reads from `stdin` interactively. It does not support a script file argument.

---

## Examples

```bash
# Simple pipeline
minishell > echo hello | cat -e
hello$

# Output redirection
minishell > ls -la > out.txt
minishell > cat out.txt

# Append redirection
minishell > echo "line1" > file.txt
minishell > echo "line2" >> file.txt

# Input redirection
minishell > cat < file.txt

# Heredoc
minishell > cat << EOF
heredoc> hello
heredoc> world
heredoc> EOF
hello
world

# Variable expansion
minishell > export NAME=world
minishell > echo "hello $NAME"
hello world

# Exit code propagation
minishell > ls /nonexistent
ls: /nonexistent: No such file or directory
minishell > echo $?
2

# Multi-stage pipeline with exit code of last command
minishell > false | true | false
minishell > echo $?
1
```

---

## Known Limitations

| Feature | Status | Notes |
|---------|--------|-------|
| Wildcard expansion (`*`, `?`) | Not implemented | Would require directory scanning and pattern matching in the expander |
| Logical operators (`&&`, `\|\|`) jobs`) | Not implemented | Requires `SIGTSTP`/`SIGCONT` handling and a job table |
| Subshell `(cmd)` syntax | Not implemented | Requires parser support for grouped commands |
| `~` expansion | Not implemented | `$HOME` expansion works; bare `~` does not |
| POSIX-compliant word splitting | Partial | IFS splitting on unquoted expansions is not fully implemented |

---

## References

- [POSIX Shell & Utilities specification](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`, `man 2 waitpid`
- `man 3 readline`, `man 3 add_history`
- `man 7 signal`, `man 2 sigaction`
- *Advanced Programming in the UNIX Environment* — W. Richard Stevens
- *The Linux Programming Interface* — Michael Kerrisk

---

## Author

**cmaubert** — 42 Paris| Not implemented | Would require a conditional execution layer in the executor |
| Job control (`bg`, `fg`, `
