/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:40:50 by dmusulas          #+#    #+#             */
/*   Updated: 2024/12/02 00:19:22 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This function validates the argument for the export command in the shell.
 * It checks if the argument starts with a valid identifier character (alphabet, 
 * underscore, or hyphen) and ensures that the rest of the characters are valid 
 * for an environment variable name. This function is crucial for maintaining 
 * the integrity of environment variables in the shell, which are used throughout 
 * the project for various built-in commands and functionalities.
 */
int	export_validation(char *arg, t_tools *tools) /* Function to validate export arguments */
{
	int	i; /* Index variable for iterating through the argument */

	if (!ft_isalpha(arg[0]) && arg[0] != '_' && arg[0] != '-') /* Check if the first character is not a valid identifier */
	{
		ft_error(ERR_INVALID_IDENTIFIER, tools); /* Report an error for invalid identifier */
		return (0); /* Return 0 indicating validation failed */
	}
	i = 1; /* Initialize index to 1 to check subsequent characters */
	while (arg[i] && arg[i] != '=') /* Loop through the argument until '=' or end of string */
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[0] != '-') /* Check if the character is not alphanumeric or valid */
		{
			ft_error(ERR_INVALID_IDENTIFIER, tools); /* Report an error for invalid identifier */
			return (0); /* Return 0 indicating validation failed */
		}
		i++; /* Move to the next character */
	}
	return (1); /* Return 1 indicating validation succeeded */
}

/* 
 * This function processes an argument for the export command.
 * It first validates the argument using the export_validation function. 
 * If the argument is valid and contains an '=', it updates or adds the 
 * environment variable in the tools structure. This function is essential 
 * for managing environment variables, which are critical for the shell's 
 * operation and are used by other built-in commands.
 */
static int	process_export_arg(char *arg, t_tools *tools) /* Function to process export arguments */
{
	int	success; /* Variable to track success of the operation */

	success = 0; /* Initialize success to 0 */
	if (!export_validation(arg, tools)) /* Validate the argument */
		success = 1; /* If validation fails, set success to 1 */
	else if (ft_strchr(arg, '=')) /* Check if the argument contains an '=' character */
	{
		if (update_or_add_envp(&tools->envp, arg) != 0) /* Update or add the environment variable */
			success = 1; /* If the update fails, set success to 1 */
		ft_lstsort(&tools->envp, cmp_envp); /* Sort the environment variables list */
	}
	return (success); /* Return the success status */
}

/* 
 * This function implements the export built-in command for the shell.
 * It processes each argument provided to the export command, validating 
 * and updating the environment variables as necessary. If no arguments 
 * are provided, it displays the current environment variables. This 
 * function is integral to the shell's functionality, allowing users 
 * to manage their environment variables effectively.
 */
int	ft_export(t_ast *cmd_node, t_tools *tools) /* Function to execute the export command */
{
	t_ast	*current; /* Pointer to traverse the command node arguments */
	int		success; /* Variable to track success of the operation */

	success = 0; /* Initialize success to 0 */
	current = cmd_node; /* Set current to the command node */
	if (!current) /* Check if there are no arguments */
	{
		ft_env(tools); /* If no arguments, display the current environment variables */
		return (success); /* Return success status */
	}
	while (current) /* Loop through each argument in the command node */
	{
		success = process_export_arg(current->str, tools); /* Process the current argument */
		if (success && ft_strchr(current->str, '=')) /* If processing was successful and contains '=' */
			return (success); /* Return success status */
		current = current->right; /* Move to the next argument */
	}
	return (success); /* Return success status */
}
