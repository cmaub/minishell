# Minishell

A minimal Unix shell implementation written in C.

## Overview

This project reproduces core behaviors of a POSIX shell, including:
- command parsing
- pipeline
- redirection
- environment management
- signal handling
- process execution

The objective is to understand how shells interact with the Unix kernel through processes, file descriptors, and system calls.

## Features

- Interactive prompt
- Environement variable expansion
- Pipeline (`|`)
- Input/output redirection (`<`, `>`, `>>`, `<<`)
- Signal handling (`SIGINT`, `SIGQUIT`)
- PATH resolution
- Exit status propagation

## Architecture

The project is divided into several subsystems:

- Lexer
    - Tokenizes user input
- Parser
    - Builds execution structures
- Expander
    - Resolves environment variables
- Executor
    - Handles process creation and execution
- Builtins
    - Internal shell commands
- Signal subsystem
    - Interactive signal managment

### Execution Flows

input 
-> lexer
-> parser
-> expander
-> executor
-> waitpid/status collection

