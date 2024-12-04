/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:12:32 by dmusulas          #+#    #+#             */
/*   Updated: 2024/11/28 16:24:51 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

/**
 * Duplicates the environment variables from tools->envp_org into a linked list.
 *
 * @param tools Pointer to a tools structure that contains the original envp.
 * @return 1 if successful, 0 if memory allocation fails.
 */
int	duplicate_env(t_tools *tools) // Function to duplicate environment variables.
{
	t_list	*envp_l; // Pointer to the head of the new linked list for environment variables.
	t_list	*new_node; // Pointer for creating new nodes in the linked list.
	int		i; // Index variable for iterating through the original environment variables.
	char	*content; // Pointer to hold the duplicated string of an environment variable.

	envp_l = ft_lstnew(tools->envp_org[0]); // Create a new list node with the first environment variable.
	if (!envp_l) // Check if the node creation failed.
		return (0); // Return 0 to indicate failure in memory allocation.
	i = 1; // Start index from 1 to skip the first variable which is already added.
	while (tools->envp_org[i]) // Loop through the original environment variables until a NULL is encountered.
	{
		content = ft_strdup(tools->envp_org[i]); // Duplicate the current environment variable string.
		new_node = ft_lstnew(content); // Create a new list node with the duplicated string.
		if (!new_node) // Check if the new node creation failed.
			return (0); // Return 0 to indicate failure in memory allocation.
		ft_lstadd_back(&envp_l, new_node); // Add the new node to the end of the linked list.
		i++; // Increment the index to process the next environment variable.
	}
	tools->envp = envp_l; // Assign the newly created linked list to tools->envp.
	return (1); // Return 1 to indicate successful duplication of environment variables.
}

/**
 * Compare two environment variable strings based on their key (before '=').
 *
 * @param a Pointer to the first environment variable string.
 * @param b Pointer to the second environment variable string.
 * @return A negative, 0, or positive value based on lexicographical comparison.
 */
int	cmp_envp(void *a, void *b)
{
	char	*env1;
	char	*env2;
	char	*key1;
	char	*key2;

	env1 = (char *)a;
	env2 = (char *)b;
	key1 = ft_strchr(env1, '=');
	key2 = ft_strchr(env2, '=');
	if (!key1 || !key2)
		return (0);
	return (ft_strncmp(env1, env2, (size_t)(key1 - env1)));
}

/* 
 * This function updates an existing environment variable in a linked list 
 * or adds a new one if it does not exist. It is part of the environment 
 * management system in the minishell project, which handles environment 
 * variables for the shell's execution context.
 */
static int	update_existing_envp(t_list *lst, const char *new_envp,
	char *new_content, size_t var_len) /* Function to update or add an environment variable. */
{
	t_list	*cur; /* Pointer to traverse the linked list of environment variables. */

	cur = lst; /* Initialize the current pointer to the head of the list. */
	while (cur) /* Loop through the linked list until the end is reached. */
	{
		if (ft_strncmp(cur->content, new_envp, var_len) == 0 /* Compare the current node's content with the new environment variable. */
			&& ((char *)cur->content)[var_len] == '=') /* Check if the current content has an '=' character right after the variable name. */
		{
			free(cur->content); /* Free the old content of the environment variable. */
			cur->content = new_content; /* Update the current node's content with the new content. */
			return (EXIT_SUCCESS); /* Return success if the variable was updated. */
		}
		cur = cur->next; /* Move to the next node in the list. */
	}
	return (EXIT_FAILURE); /* Return failure if the variable was not found in the list. */
}

int	update_or_add_envp(t_list **lst, const char *new_envp) /* Function to update or add an environment 
variable to the list. */
{
	char	*equal_sign; /* Pointer to locate the '=' character in the environment variable string. */
	size_t	var_len; /* Length of the variable name (before the '=' character). */
	char	*new_content; /* Pointer to hold the duplicated string of the new environment variable. */
	t_list	*new_node; /* Pointer for creating a new node in the linked list. */

	equal_sign = ft_strchr(new_envp, '='); /* Find the position of the '=' character in the new environment variable. */
	if (!equal_sign) /* Check if the '=' character was not found. */
		return (EXIT_FAILURE); /* Return failure if the format is incorrect. */
	var_len = equal_sign - new_envp; /* Calculate the length of the variable name. */
	new_content = ft_strdup(new_envp); /* Duplicate the new environment variable string. */
	if (!new_content) /* Check if the duplication failed. */
		return (-1); /* Return -1 to indicate memory allocation failure. */
	if (update_existing_envp(*lst, new_envp, new_content, var_len) /* Try to update the existing environment variable. */
		== EXIT_SUCCESS) /* Check if the update was successful. */
		return (EXIT_SUCCESS); /* Return success if the variable was updated. */
	new_node = ft_lstnew(new_content); /* Create a new list node with the duplicated string. */
	if (!new_node) /* Check if the new node creation failed. */
	{
		free(new_content); /* Free the duplicated string if node creation failed. */
		return (-1); /* Return -1 to indicate memory allocation failure. */
	}
	ft_lstadd_front(lst, new_node); /* Add the new node to the front of the linked list. So that we
	do not need to traverse two times, once for lstadd and once for lstsort*/
	return (EXIT_SUCCESS); /* Return success after adding the new environment variable. */
}