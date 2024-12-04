/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 22:33:09 by pmolzer           #+#    #+#             */
/*   Updated: 2024/12/01 18:51:06 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This function prints an argument with variable expansion for the echo command.
 * It checks if the argument contains a special variable (like the last exit status)
 * and prints it accordingly. This function is part of the echo built-in command,
 * which is used to display text or variables in the shell.
 */
static void	print_argument_with_expansion(t_ast *current, t_tools *tools) /* Function to print argument with variable expansion */
{
	char	*str; /* Pointer to the string of the current argument */
	int		i; /* Index for iterating through the string */

	str = current->str; /* Get the string from the current AST node */
	i = 0; /* Initialize index to 0 */
	if (!tools->in_single_quotes) /* Check if we are not in single quotes */
	{
		while (str[i]) /* Iterate through each character in the string */
		{
			if (str[i] == '$' && str[i + 1] == '?') /* Check for the special variable $? */
			{
				ft_putnbr_fd(tools->last_exit_status, STDOUT_FILENO); /* Print the last exit status */
				i += 2; /* Move index past the special variable */
			}
			else
				ft_putchar_fd(str[i++], STDOUT_FILENO); /* Print the current character */
		}
	}
	else /* If we are in single quotes */
	{
		while (str[i]) /* Iterate through each character in the string */
			ft_putchar_fd(str[i++], STDOUT_FILENO); /* Print the current character */
	}
}

/* 
 * This function prints an argument, handling the special case for the last exit status.
 * It checks if the argument is the special variable "$?" and prints its value if so.
 * Otherwise, it calls another function to handle variable expansion.
 */
static void	print_argument(t_ast *current, t_tools *tools) /* Function to print argument */
{
	if (!tools->in_single_quotes && ft_strcmp(current->str, "$?") == 0) /* Check if not in single quotes and argument is "$?" */
		ft_putnbr_fd(tools->last_exit_status, STDOUT_FILENO); /* Print the last exit status */
	else
		print_argument_with_expansion(current, tools); /* Call function to print with expansion */
}

/* 
 * This function implements the echo built-in command, which prints arguments to the output.
 * It handles the "-n" option to suppress the newline at the end of the output.
 * It iterates through the arguments and prints each one, adding spaces between them.
 */
int	ft_echo(t_ast *cmd_node, t_tools *tools) /* Function to execute the echo command */
{
	t_ast	*current; /* Pointer to the current argument node */
	int		n_line; /* Flag to determine if a newline should be printed */

	current = cmd_node->right; /* Get the right child of the command node, which contains arguments */
	n_line = 1; /* Initialize the newline flag to true */
	if (current && ft_strcmp(current->str, "-n") == 0) /* Check if the first argument is "-n" */
	{
		n_line = 0; /* Set the newline flag to false */
		current = current->right; /* Move to the next argument */
	}
	while (current) /* Iterate through the arguments */
	{
		print_argument(current, tools); /* Print the current argument */
		if (current->right) /* If there is another argument */
			ft_putchar_fd(' ', STDOUT_FILENO); /* Print a space */
		current = current->right; /* Move to the next argument */
	}
	if (n_line) /* If the newline flag is true */
		ft_putchar_fd('\n', STDOUT_FILENO); /* Print a newline */
	return (0); /* Return 0 to indicate success */
}
