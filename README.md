# minishell

A small, educational Unix shell implemented in C. It reproduces a focused subset of Bash behavior: interactive prompt, command execution, pipes, redirections (including here-doc), environment variables and the required built-ins—built with robust parsing and careful signal/memory handling.

> This repository is part of the 42 curriculum “minishell” project.

## Features

- Interactive prompt: `minishell$` (with GNU Readline line-editing and history)
- Execute programs by name via `$PATH`, or via absolute/relative paths
- Pipes: `cmd1 | cmd2 | ...`
- Redirections: `<`, `>`, `>>`, and here-doc `<<`
- Environment variables expansion, including `$?` (last exit status)
- Quote-aware parsing (single quotes inhibit expansion, double quotes allow it)
- Built-in commands (no forking when appropriate)
- Correct signal behavior in interactive mode (Ctrl-C, Ctrl-\)
- Exit status propagation consistent with a POSIX shell subset

## Supported built-ins

- `echo` with `-n`
- `cd` (updates `PWD`/`OLDPWD`)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

## Requirements

- Linux
- GCC or Clang
- GNU Readline (headers and library)

Install Readline on common distros:

- Debian/Ubuntu: `sudo apt-get update && sudo apt-get install -y libreadline-dev`
- Fedora: `sudo dnf install -y readline-devel`
- Arch: `sudo pacman -S readline`

## Build

The project ships with a `Makefile` that produces the `minishell` binary and links with Readline.

```sh
make
```

Common targets:

- `make` or `make all` — build `minishell`
- `make clean` — remove object files
- `make fclean` — remove objects and binary
- `make re` — rebuild from scratch

## Run

```sh
./minishell
```

You should see a cyan prompt:

```
minishell$ 
```

## Usage examples

- Run a program:
  - `ls -la`
- Pipeline:
  - `cat file.txt | grep keyword | wc -l`
- Redirections:
  - `echo hello > out.txt`
  - `cat < in.txt`
  - `echo another >> out.txt`
- Here-doc (stops at delimiter on its own line):
  - `cat << EOF`
    Then type lines… finish with `EOF`.
- Environment expansion:
  - `echo $HOME`
  - `echo "user: $USER"`
  - `echo $?` (prints last command’s exit status)
- Built-ins:
  - `echo -n "no newline"`
  - `pwd`
  - `cd /tmp`
  - `export FOO=bar`
  - `env`
  - `unset FOO`
  - `exit 42`

## Behavior and compatibility

- Quotes:
  - Single quotes `'…'` preserve literal text (no expansion)
  - Double quotes "…" allow `$VAR` and `$?` expansion
- Signals:
  - `Ctrl-C` (SIGINT) on an empty prompt moves to a new line and returns prompt
  - `Ctrl-\` (SIGQUIT) is ignored in the interactive prompt
  - Correct handling inside child processes, pipelines, and here-doc
- Exit status:
  - Stored and exposed as `$?`
  - Built-ins follow Bash-like return codes (e.g., `exit` validation, `cd` errors)

## Project structure (high level)

- `minishell.c` — entry point, prompt loop, signal hooks
- `minishell.h` — shared types, prototypes, and includes
- Parsing:
  - `make_input_beautiful.c`, `parse_utils.c`, `command_parsing.c`, `ft_split.c`, `ft_second_split.c`
  - Quote handling and tokenization
- Expansion:
  - `variable_expand.c` — `$VAR` and `$?`, quote-aware
- Redirections:
  - `parse_redirection.c`, `parse_redirection_helpers.c` — `<`, `>`, `>>`, `<<`
  - `here_doc.c` — here-document support
- Execution:
  - `commands_routine.c`, `pipe_command_routine.c`, `check_command.c`, `commad_compilation.c`
- Built-ins:
  - `built-in.c`, `built_in_helpers.c`, `ft_echo.c`, `ft_exit.c`, `ft_export.c`, `ft_unset.c`, `ft_pwd.c`
- Utilities and memory:
  - `helpers.c`, `utils.c`, `utils_two.c`
  - `garbage_collector.c`, `initialize_variable.c` — centralized allocation and cleanup
  - `libft_functions*.c` — minimal libft subset used internally

File names above map directly to the sources listed in the `Makefile`.

## Implementation notes

- Readline provides history and line-editing; we link with `-lreadline`.
- Built-ins are detected and run in-process when possible; others are execve’d with proper `PATH` resolution.
- Here-docs are implemented with a child writer and temporary file/FD, with signal-aware behavior.
- A small garbage-collector style list tracks allocations to simplify clean shutdowns and error unwinding.

## Known limitations (by design for this project)

- No job control (bg/fg), `&&`/`||`, command substitution, or subshells `(...)`
- No globbing (wildcards `*`) expansion
- Limited quoting and escaping semantics to the project’s scope

## Troubleshooting

- Missing Readline headers/libraries during build:
  - Install the development package for your distro (see Requirements), then `make re`.
- If `minishell` doesn’t run:
  - Ensure `./minishell` exists and is executable after a successful `make`.

## Authors

- StegenX — https://github.com/StegenX
- zm-x — https://github.com/zm-x
