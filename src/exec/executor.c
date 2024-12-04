/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 17:35:54 by dmusulas          #+#    #+#             */
/*   Updated: 2024/12/01 19:04:45 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_messages.h"
#include "exec.h"
#include "lexer_parser.h"
#include "minishell.h"

/**
 * @brief Executes a single command from the Abstract Syntax Tree (AST)
 * 
 * This function is responsible for executing a single command node from the AST.
 * It differentiates between built-in commands and external commands:
 * - For built-in commands (like cd, echo, etc.), it executes them directly
 * - For external commands (like ls, cat, etc.), it creates a new process
 *
 * @param node The AST node containing command information
 * @param tools Structure containing shell utilities and state
 */
void	execute_single_command(t_ast *node, t_tools *tools)
{
	/* Check if the command is a built-in shell command */
	if (node->b_cmd)
		execute_builtin(node, tools);
	else
		/* For external commands, create a new process */
		fork_and_execute_command(node, tools);
}

/**
 * @brief Executes an external command at a specified path
 * 
 * This function handles the actual execution of external commands using execve.
 * It prepares the command arguments and environment variables before execution.
 * If execution fails, it handles the error appropriately.
 *
 * @param path Full path to the executable
 * @param node AST node containing command details
 * @param envp Environment variables array
 * @param tools Shell utilities and state
 */
void	execute_at_path(char *path, t_ast *node, char **envp, t_tools *tools)
{
	char	**cmd_args;
	int		exec_status;

	/* Parse and prepare command arguments */
	cmd_args = parse_cmd_args(path, node);
	/* Execute the command with execve */
	exec_status = execve(path, cmd_args, envp);
	
	/* Handle execution failure */
	if (exec_status == -1)
	{
		ft_error(ERR_EXECVE_FAIL, tools);
		exit(126);  /* Exit with error status if execve fails */
	}
	tools->last_exit_status = exec_status;
}

/**
 * @brief Validates and handles environment variable directory checks
 * 
 * This function checks if an environment variable points to a directory
 * and handles appropriate error cases. It's used to prevent execution
 * of directories as commands.
 *
 * @param node Current AST node
 * @param tools Shell utilities and state
 */
void	check_env_directory(t_ast *node, t_tools *tools)
{
	char	*env_value;
	DIR		*dir;

	/* Skip if node is invalid or not an environment variable */
	if (!node || !node->str || node->str[0] != '$')
		return ;
	
	/* Skip special environment variables */
	if (is_special_env_var(node->str))
		return ;
	
	/* Get the value of the environment variable */
	env_value = get_env_value(node->str + 1, tools);
	if (!env_value)
	{
		handle_empty_env(node, tools);
		return ;
	}
	
	/* Check if the environment value points to a directory */
	dir = opendir(env_value);
	if (dir)
	{
		closedir(dir);
		handle_directory_error(env_value, tools);
	}
}

/**
 * @brief Main command execution handler
 * 
 * This is the primary function for executing commands in the shell.
 * It handles different types of commands and operations:
 * - Pipe operations
 * - Environment variable expansions
 * - Directory checks
 * - Command execution with redirections
 *
 * The function also manages signal handling during command execution.
 *
 * @param node Current AST node to execute
 * @param tools Shell utilities and state
 */
void	execute_command(t_ast *node, t_tools *tools)
{
	char	*env_value;

	/* Set signals to inactive mode during command execution */
	set_inactive_signals();
	
	if (!node)
		return ;

	/* Handle pipe operations */
	if (node->token == T_PIPE)
	{
		handle_pipes(node, tools);
		return ;
	}

	/* Handle environment variable expansion */
	if (node->str && node->str[0] == '$')
	{
		if (ft_strcmp(node->str, "$") != 0 && ft_strcmp(node->str, "$?") != 0)
		{
			env_value = get_env_value(node->str + 1, tools);
			if (!env_value && node->right)
			{
				execute_command(node->right, tools);
				return ;
			}
		}
	}

	/* Check for directory-related issues */
	check_env_directory(node, tools);
	
	/* Execute the command with any redirections */
	execute_node_with_redirects(node, tools);
	
	/* Restore active signal handling */
	set_active_signals();
}

/*Sure! Let's break down the signal handling in the `minishell` program with two specific examples: executing the command `ls` and executing the command `cat file1`. We'll look at how signals are handled in both active and inactive modes.

Signal Handling Overview

1. **Active Mode**: 
   - The shell is waiting for user input.
   - The signal handler for `SIGINT` (Ctrl+C) is set to allow the shell to gracefully handle interruptions. When Ctrl+C is pressed, the shell will print a newline, clear the current input line, and redisplay the prompt.

2. **Inactive Mode**: 
   - The shell is executing a command.
   - The signal handler for `SIGINT` is set to allow the command to terminate gracefully. If Ctrl+C is pressed, the command will be interrupted, and the shell will not interfere with the command's execution.

Example 1: Executing `ls`

1. **User Input**: The user types `ls` and presses Enter.
2. **Signal Handling**:
   - The shell is in **inactive mode** when executing the command.
   - The signal handler for `SIGINT` is set to allow the `ls` command to terminate if interrupted.
3. **Behavior**:
   - If the user presses Ctrl+C while `ls` is running, the `SIGINT` signal is sent to the `ls` process.
   - The `ls` command receives the signal and terminates immediately. The shell does not 
   print a newline or clear the input line because it is not waiting for user input; it is executing a command.
   - After `ls` finishes (either normally or due to interruption), the shell will return to the prompt.

Example 2: Executing `cat file1`

1. **User Input**: The user types `cat file1` and presses Enter.
2. **Signal Handling**:
   - The shell is again in **inactive mode** when executing the command.
   - The signal handler for `SIGINT` is set to allow the `cat` command to terminate if interrupted.
3. **Behavior**:
   - If the user presses Ctrl+C while `cat file1` is running, the `SIGINT` signal is sent to the `cat` process.
   - The `cat` command receives the signal and terminates immediately. The shell does not print a 
   newline or clear the input line for the same reason as before.
   - After `cat` finishes (either normally or due to interruption), the shell will return to the prompt.

Summary of Differences in Signal Handling

- **Active Mode** (when waiting for input):
  - Ctrl+C results in the shell handling the interruption, printing a newline, clearing the input line, 
  and redisplaying the prompt.
  
- **Inactive Mode** (when executing commands):
  - Ctrl+C sends the signal to the currently running command (like `ls` or `cat`), allowing it to 
  terminate immediately without affecting the shell's prompt or input line.

Key Points

- For both `ls` and `cat file1`, the shell is in inactive mode during execution, meaning that pressing 
Ctrl+C will interrupt the command rather than the shell itself.
- The difference lies in how the shell responds to user input versus command execution. In active mode, 
the shell manages user interruptions, while in inactive mode, it allows commands to handle their own interruptions. 

This distinction is crucial for providing a responsive user experience in a shell environment.*/
