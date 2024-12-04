/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 22:37:06 by pmolzer           #+#    #+#             */
/*   Updated: 2024/12/01 18:45:22 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This function prints all environment variables stored in the tools structure.
 * It iterates through the linked list of environment variables and writes each
 * variable to the standard output, followed by a newline. This function is part
 * of the builtins module, which manages built-in commands in the shell, and 
 * interacts with the environment variables to display their current values.
 */
int	ft_env(t_tools *tools) /* Function to display the environment variables */
{
	t_list	*current; /* Pointer to traverse the linked list of environment variables */
	char	*env_var; /* Pointer to store the current environment variable string */

	current = tools->envp; /* Initialize current to the head of the environment variable list */
	while (current) /* Loop through each environment variable in the list */
	{
		env_var = (char *)current->content; /* Get the content of the current node as a string */
		write(STDOUT_FILENO, env_var, ft_strlen(env_var)); /* Write the environment variable to standard output */
		write(STDOUT_FILENO, "\n", 1); /* Write a newline character to standard output */
		current = current->next; /* Move to the next environment variable in the list */
	}
	return (0);
}
