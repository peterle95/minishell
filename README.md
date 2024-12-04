# Minishell

<p align="center">
  <img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/minishelle.png" alt="Minishell 42 project badge"/>
</p>

## Project Description

`minishell` is a simple Unix shell implementation designed to provide a lightweight and customizable command-line interface. It aims to replicate the basic functionalities of a shell, allowing users to execute commands, manage processes, and handle input/output redirection.

### Features

- **Command Execution**: Execute built-in and external commands.
- **Environment Variable Management**: Set, get, and delete environment variables.
- **Input/Output Redirection**: Support for input and output redirection using `<`, `>`, and `|`.
- **Signal Handling**: Custom signal handling for a better user experience, including handling of `Ctrl+C` and `Ctrl+D`.
- **Command History**: Maintain a history of executed commands for easy access and re-execution.
- **Error Handling**: Robust error messages for common issues like command not found, syntax errors, and memory allocation failures.

### Installation

To install `minishell`, clone the repository and compile the project using the provided Makefile:

```bash
git clone https://github.com/peterle95/minishell.git
cd minishell
make
```

### Usage

After compiling, you can run the shell by executing the following command in your terminal:

```bash
./minishell
```

You can then start entering commands as you would in a typical Unix shell.

---
Made by pmolzer and [dmusulas](https://github.com/Dmusulas): pmolzer@student.42berlin.de | LinkedIn: [pmolzer](https://www.linkedin.com/in/peter-moelzer//) 
