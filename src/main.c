/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:00:49 by dmusulas          #+#    #+#             */
/*   Updated: 2024/12/01 18:50:49 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
rl_catch_signals:
	if not disabled Readline internal sig handling messes up our sig handler
	BUT leaves memory blocks still accessible....
*/
/* 
 * The main function serves as the entry point for the minishell program. 
 * It initializes the necessary tools and environment for the shell, 
 * sets up signal handling, and enters the main loop for processing user commands. 
 * This file interacts with other components of the project, such as 
 * environment variable management and command execution, by utilizing 
 * the t_tools structure to maintain state and configuration throughout 
 * the shell's operation.
 */
int	main(int argc, char *argv[], char *envp[]) /* Main function declaration with command-line arguments */
{
	t_tools	tools; /* Declare a tools structure to hold environment and state information */

	(void)argc; /* Ignore the argc parameter as it's not used */
	(void)argv; /* Ignore the argv parameter as it's not used */
	tools.envp_org = envp; /* Store the original environment variables in the tools structure */
	rl_catch_signals = 0; 
	/* Disable Readline's internal signal handling to allow custom handling.
	
	When rl_catch_signals is set to 1 (default):
		Readline handles signals like SIGINT (Ctrl+C) internally
		This can interfere with our own signal handlers
		It can cause memory management issues because readline's internal 
		handling might not properly clean up our program's resources
	When we set rl_catch_signals = 0:
		We disable readline's internal signal handling
		This allows our program to handle signals properly with our own handlers
		We maintain better control over program cleanup and memory management
		We can implement custom behavior for signals like SIGINT
	This is particularly important in a shell implementation because:
		We need precise control over how signals are handled in both the parent shell and child processes
		We want to ensure proper cleanup of resources when signals are received
		We need to maintain consistent shell behavior when handling interrupts
	Without setting rl_catch_signals = 0, you might encounter issues like:
		Inconsistent behavior when handling Ctrl+C
		Memory leaks when the program is interrupted
		Interference with your custom signal handlers
		Unexpected program state after signal handling*/
	if (!duplicate_env(&tools)) /* Attempt to duplicate the environment variables */
		return (ft_error(ERR_MALLOC, &tools)); /* If duplication fails, return an error */
	init_tools(&tools); /* Initialize other tools and settings in the tools structure */
	set_initial_exit_status(&tools); /* Set the initial exit status for the shell */
	mini_loop(&tools); /* Enter the main loop to process user commands */
	if (tools.envp != NULL) /* Check if the duplicated environment exists */
		delete_duplicated_envp(&tools); /* Clean up the duplicated environment variables */
	return (0); /* Return 0 to indicate successful execution */
}
