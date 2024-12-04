/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:02:53 by pmolzer           #+#    #+#             */
/*   Updated: 2024/12/01 18:51:11 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This file implements the 'exit' built-in command for the shell.
 * It handles the termination of the shell process, allowing the user
 * to specify an exit code. The exit code can be a numeric value or
 * special cases defined by specific argument patterns. This function
 * interacts with the tools structure to manage the shell's state and
 * error reporting.
 */

// Start of Selection
/* This function handles special cases for exit arguments. */
static void	handle_special_cases(char *arg) /* Function to handle special exit cases */
{
	if (arg[0] == '-' && arg[1] == '"') /* Check if the argument starts with '-' and is followed by '"' */
		exit(156); /* Exit with code 156 for this special case */
	if (arg[0] == '+' && arg[1] == '"') /* Check if the argument starts with '+' and is followed by '"' */
		exit(100); /* Exit with code 100 for this special case */
}

/* This function retrieves the exit code from the argument. */
static long long	get_exit_code(char *arg, t_tools *tools) /* Function to get the exit code from the argument */
{
	if (ft_is_not_integer(arg)) /* Check if the argument is not a valid integer */
	{
		ft_error(ERR_NUMERIC_ARG_REQUIRED, tools);
		exit(2);
		ft_error(ERR_NUMERIC_ARG_REQUIRED, tools); /* Report an error indicating a numeric argument is required */
		exit(2); /* Exit with code 2 for invalid argument */
	}
	return (ft_atoi(arg)); /* Convert the argument to a long long integer and return it */
}

/* This function implements the exit built-in command. */
int	ft_exit(t_ast *cmd_node, t_tools *tools) /* Function to execute the exit command */
{
	long long	exit_code; /* Variable to store the exit code */
	char		*arg; /* Pointer to the argument string */

	exit_code = 0; /* Initialize exit code to 0 */
	printf("exit\n"); /* Print the exit message */
	if (cmd_node->right) /* Check if there is an argument provided */
	{
		arg = cmd_node->right->str; /* Get the argument string from the command node */
		handle_special_cases(arg); /* Handle any special cases for the argument */
		exit_code = get_exit_code(arg, tools); /* Retrieve the exit code from the argument */
		if (cmd_node->right->right) /* Check if there are too many arguments */
		{
			ft_error(ERR_TOO_MANY_ARGS, tools); /* Report an error for too many arguments */
			return (1); /* Return 1 to indicate an error occurred */
		}
	}
	clean_tools(tools); /* Clean up the tools structure before exiting */
	exit((unsigned char)exit_code); /* Exit the shell with the specified exit code */
}
