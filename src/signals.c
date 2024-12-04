/*
 * This file implements signal handling for the minishell program.
 * 
 * Signal handling is crucial for managing user interactions and ensuring
 * the shell behaves correctly in response to signals like SIGINT (Ctrl+C)
 * and SIGQUIT (Ctrl+\). The shell operates in two modes: active and inactive.
 * 
 * Active Mode:
 * - This mode is typically used when the shell is waiting for user input.
 * - In this mode, the signal handler for SIGINT is set to handle interruptions
 *   gracefully. When the user presses Ctrl+C, the shell will:
 *   1. Print a newline to the terminal.
 *   2. Clear the current input line.
 *   3. Redisplay the prompt for the user to enter a new command.
 * - This allows the user to cancel their current input without terminating the shell.

 * Inactive Mode:
 * - This mode is used when the shell is executing commands or in a non-interactive
 *   state (e.g., when running scripts).
 * - In this mode, the signal handler for SIGINT simply prints a newline, allowing
 *   the command to terminate gracefully.
 * - The handler for SIGQUIT prints a "Quit" message along with the signal number,
 *   indicating that the command was terminated by the user.
 * 
 * The setup_signals function is responsible for configuring the signal handlers
 * based on the provided handler function (either active or inactive). The shell
 * can switch between these modes as needed, ensuring appropriate behavior
 * depending on the context of execution.
 * 
 * 

### Signal Handling Overview

In the `minishell` program, signal handling is managed through two primary functions: 
`set_active_signals` and `set_inactive_signals`. 
These functions configure the signal handlers for different modes of operation.

### Active Mode

- **Function**: `set_active_signals`
- **Handler**: `handle_active_signal`
- **Usage**: This mode is activated when the shell is waiting for user input. 
The signal handler for `SIGINT` (triggered by Ctrl+C) is set to `handle_active_signal`, 
which performs the following actions:
  - Prints a newline to the terminal.
  - Clears the current input line.
  - Redisplays the prompt for the user to enter a new command.

**Code Snippet**:

void	set_active_signals(void)
{
	setup_signals(handle_active_signal);
}

**When Used**: 
- This function is called in `init_signals` to initialize the signal handling when the shell starts up:

void	init_signals(void)
{
	set_active_signals();
}

### Inactive Mode

- **Function**: `set_inactive_signals`
- **Handler**: `handle_inactive_signal`
- **Usage**: This mode is used when the shell is executing commands or is in a non-interactive state (e.g., running scripts). The signal handler for `SIGINT` simply prints a newline, allowing the command to terminate gracefully. The handler for `SIGQUIT` prints a "Quit" message along with the signal number.

**Code Snippet**:
void	set_inactive_signals(void)
{
	setup_signals(handle_inactive_signal);
}

**When Used**: 
- This function is typically called when the shell is about to execute a command. 
For example, in the `execute_command` function, after forking a new process, 
the shell can switch to inactive mode to handle signals appropriately during command execution:
void	execute_command(t_ast *node, t_tools *tools)
{
	// ... other code ...
	set_inactive_signals();
	// Execute the command
}

### Summary of Usage

1. **Active Mode**:
   - Set up by calling `set_active_signals()` in `init_signals()`.
   - Used when the shell is waiting for user input, allowing for graceful interruption of input.

2. **Inactive Mode**:
   - Set up by calling `set_inactive_signals()` before executing commands.
   - Used during command execution to handle signals without interrupting the command flow.
 */

#include "minishell.h"

/**
 * Handles signals in interactive mode, specifically SIGINT.
 * - Prints a newline
 * - Clears the current line
 * - Redisplays the prompt.
 *
 * @param signal The signal received.
 */
static void	handle_active_signal(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

/**
 * Handles signals in non-interactive mode.
 * - Prints a newline for SIGINT.
 * - Prints a "Quit" message for SIGQUIT along with the signal number.
 *
 * @param signal The signal received.
 */
static void	handle_inactive_signal(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit: ", STDERR_FILENO);
		ft_putnbr_fd(signal, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
}

/*
 * This function sets up signal handling by defining the handler for SIGINT and SIGQUIT signals
 * using the provided handler function. It initializes a sigaction structure, sets the desired
 * signal handler, and applies it to the specified signals.
 *
 * The function is called by:
 * - `set_active_signals()`: This sets the handler to `handle_active_signal`, which manages
 *   user interruptions gracefully when the shell is waiting for input.
 * - `set_inactive_signals()`: This sets the handler to `handle_inactive_signal`, which allows
 *   commands to terminate gracefully when the shell is executing commands.
 * 
 * ACTIVE MODE:
 * - When `setup_signals` is called by `set_active_signals`, the `handler` parameter is set to 
 * `handle_active_signal`. This means that when a user sends a `SIGINT` (e.g., by pressing Ctrl+C), 
 * the shell will execute the logic defined in `handle_active_signal`, which includes printing a newline, 
 * clearing the current input line, and redisplaying the prompt. This allows the user to cancel their 
 * current input without terminating the shell.
 * 
 * INACTIVE MODE:
 * - When `setup_signals` is called by `set_inactive_signals`, the `handler` parameter is set to 
 * `handle_inactive_signal`. This means that when a user sends a `SIGINT` (e.g., by pressing Ctrl+C), 
 * the shell will execute the logic defined in `handle_inactive_signal`, which includes printing a newline.
 * When a user sends a `SIGQUIT` (e.g., by pressing Ctrl+\), the shell will execute the logic defined in 
 * `handle_inactive_signal`, which includes printing a "Quit" message along with the signal number.
 * 
 * This function sets up signal handling for the specified signals (SIGINT and SIGQUIT).
 * It uses the sigaction structure to define how the program should respond to these signals.
 * 
 * - struct sigaction: This structure is used to specify the action to be associated with a specific signal.
 *   It contains at least the following members:
 *   - sa_handler: A pointer to the signal handling function that will be called when the signal is received.
 *   - sa_mask: A set of signals that should be blocked during the execution of the signal handler.
 *   - sa_flags: Flags that modify the behavior of the signal handling.
 * 
 * - sigemptyset: This function initializes the signal set to exclude all signals. It is called to ensure
 *   that no signals are blocked when the handler is executed.
 * 
 * - sigaction: This function is used to change the action taken by a process on receipt of a specific signal.
 *   It takes three arguments: the signal number, a pointer to the sigaction structure, and a pointer to
 *   a previous sigaction structure (which can be NULL if not needed). Here, it sets the handler for SIGINT
 *   and SIGQUIT to the specified handler function.
 */
static void	setup_signals(void (*handler)(int))
/*Function Pointer: The function takes a pointer to a signal handler function as an argument 
(void (*handler)(int)). This function will be called when the specified signal is received.*/
{
	struct sigaction	sa;
	/*A struct sigaction variable named sa is declared. This structure is used to specify the 
	action to be taken when a signal is received.*/

	sa.sa_handler = handler;
	/*assigns the provided handler function to the sa_handler member of the sigaction structure. 
	This tells the program which function to call when the signal is received.*/
	sigemptyset(&sa.sa_mask);
	/*The sa_mask member of the sigaction structure is used to specify a set of signals that should be blocked 
	during the execution of the signal handler. Here, sigemptyset(&sa.sa_mask) initializes the signal set to 
	exclude all signals, ensuring that no other signals are blocked when the handler is executed.*/
	sa.sa_flags = 0;
	/*The sa_flags member of the sigaction structure is used to specify additional flags that modify the behavior 
	of the signal handling. Here, sa.sa_flags is set to 0, which means no additional flags are used.*/
	sigaction(SIGINT, &sa, NULL);
	/*The sigaction function is called with the SIGINT signal, the sigaction structure, and NULL. This sets up the 
	signal handler for SIGINT.*/
	sigaction(SIGQUIT, &sa, NULL);
	/*The sigaction function is called with the SIGQUIT signal, the sigaction structure, and NULL. This sets up the 
	signal handler for SIGQUIT.*/
}

/**
 * Configures signal handling for interactive mode by setting the
 * active signal handler.
 */
void	set_active_signals(void)
{
	setup_signals(handle_active_signal);
}

/**
 * Configures signal handling for non-interactive mode by setting
 * the inactive signal handler.
 */
void	set_inactive_signals(void)
{
	setup_signals(handle_inactive_signal);
}
