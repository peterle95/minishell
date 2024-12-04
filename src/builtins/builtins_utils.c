/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:07:03 by pmolzer           #+#    #+#             */
/*   Updated: 2024/12/01 18:51:01 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This function checks if a given string represents a valid integer.
 * It returns 1 if the string is not a valid integer, and 0 otherwise.
 * This function is used in various parts of the shell to validate 
 * numeric arguments, particularly in built-in commands like 'exit'.
 */
int	ft_is_not_integer(char *str) /* Function to check if the string is not an integer */
{
	int	i; /* Index variable for iterating through the string */

	i = 0; /* Initialize index to 0 */
	if (str[0] == '-' || str[0] == '+') /* Check if the first character is a sign */
		i++; /* Increment index to skip the sign */
	while (str[i]) /* Loop through the string until the end */
	{
		if (str[i] < '0' || str[i] > '9') /* Check if the character is not a digit */
			return (1); /* Return 1 indicating the string is not a valid integer */
		i++; /* Move to the next character */
	}
	return (0); /* Return 0 indicating the string is a valid integer */
}

/* 
 * This function removes an environment variable from the linked list of 
 * environment variables. It takes a pointer to the head of the list and 
 * the name of the variable to be removed. This function is essential for 
 * managing the environment variables in the shell.
 */
void	remove_env_var(t_list **envp, const char *var_name) /* Function to remove an environment variable */
{
	t_list	*prev; /* Pointer to the previous node in the list */
	t_list	*current; /* Pointer to the current node in the list */
	char	*env_var; /* Pointer to store the current environment variable string */
	size_t	var_len; /* Length of the variable name */

	prev = NULL; /* Initialize previous pointer to NULL */
	current = *envp; /* Start with the head of the environment variable list */
	var_len = ft_strlen(var_name); /* Get the length of the variable name */
	while (current) /* Loop through the linked list */
	{
		env_var = (char *)current->content; /* Get the content of the current node as a string */
		if (ft_strncmp(env_var, var_name, var_len) == 0 /* Check if the current variable matches the name */
			&& env_var[var_len] == '=') /* Ensure the next character is '=' */
		{
			if (prev) /* If there is a previous node */
				prev->next = current->next; /* Bypass the current node */
			else
				*envp = current->next; /* Update head if the first node is being removed */
			free(current->content); /* Free the memory allocated for the variable string */
			free(current); /* Free the current node */
			return ; /* Exit the function after removal */
		}
		prev = current; /* Move previous pointer to current */
		current = current->next; /* Move to the next node */
	}
}

/* 
 * This function changes the current working directory to an absolute path.
 * It takes a path as input and updates the shell's environment accordingly.
 * If the path does not exist, it returns an error. This function is part 
 * of the shell's built-in commands for navigating the file system.
 */
int	change_to_absolute_path(char *path, t_tools *tools) /* Function to change to an absolute path */
{
	if (chdir(path) == -1) /* Attempt to change the directory */
		return (ft_path_error(ERR_NO_SUCH_FILE, tools, path)); /* Return error if it fails */
	cd_update_env_paths(tools); /* Update the environment variables related to the current directory */
	return (0); /* Return 0 indicating success */
}

/* 
 * This function changes the current working directory to a relative path.
 * It constructs the full path from the current working directory and the 
 * provided relative path, then attempts to change to that directory.
 * This function is also part of the shell's built-in commands for 
 * navigating the file system.
 */
int	change_to_relative_path(char *path, t_tools *tools) /* Function to change to a relative path */
{
	char	*cwd; /* Variable to store the current working directory */
	char	*full_path; /* Variable to store the constructed full path */

	cwd = getcwd(NULL, 0); /* Retrieve the current working directory */
	if (!cwd) /* Check if the retrieval failed */
		return (ft_path_error(ERR_NO_SUCH_FILE, tools, path)); /* Return error if it fails */
	full_path = ft_strjoin(cwd, "/"); /* Join the current directory with a '/' */
	full_path = ft_strjoin(full_path, path); /* Append the relative path to the full path */
	free(cwd); /* Free the memory allocated for the current directory string */
	if (chdir(full_path) == -1) /* Attempt to change the directory */
	{
		ft_path_error(ERR_NO_SUCH_FILE, tools, path); /* Return error if it fails */
		free(full_path); /* Free the constructed full path */
		return (1); /* Return 1 indicating failure */
	}
	free(full_path); /* Free the constructed full path */
	cd_update_env_paths(tools); /* Update the environment variables related to the current directory */
	return (0); /* Return 0 indicating success */
}
