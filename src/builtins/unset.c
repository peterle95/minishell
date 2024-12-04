/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:11:20 by pmolzer           #+#    #+#             */
/*   Updated: 2024/11/29 23:03:39 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This function implements the 'unset' built-in command for the shell.
 * It removes environment variables specified in the command node from the 
 * shell's environment. This function interacts with the tools structure 
 * to manage the environment variables, which are crucial for the shell's 
 * operation and are used by other built-in commands like 'export' and 'env'.
 */
int	ft_unset(t_ast *cmd_node, t_tools *tools) /* Function to execute the unset command */
{
	t_ast	*current; /* Pointer to traverse the command node arguments */

	current = cmd_node; /* Initialize current to the command node */
	while (current) /* Loop through each argument in the command node */
	{
		remove_env_var(&tools->envp, current->str); /* Remove the environment variable specified by current->str */
		current = current->right; /* Move to the next argument in the command node */
	}
	return (0); /* Return 0 to indicate successful completion of the function */
}
