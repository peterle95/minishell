/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:10:57 by pmolzer           #+#    #+#             */
/*   Updated: 2024/11/29 23:03:17 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * This function implements the 'pwd' built-in command for the shell.
 * It retrieves the current working directory and prints it to the standard output.
 * If there is an error in retrieving the current directory, it handles the error
 * by checking the specific error codes and returning appropriate error messages.
 * This function is part of the builtins module, which manages built-in commands
 * in the shell, and interacts with the environment variables to provide user feedback.
 */
int	ft_pwd(t_tools *tools) /* Function to execute the pwd command */
{
	char	*pwd; /* Variable to store the current working directory */

	pwd = getcwd(NULL, 0); /* Retrieve the current working directory and store it in pwd */
	if (pwd) /* Check if the current working directory was successfully retrieved */
	{
		write(STDOUT_FILENO, pwd, ft_strlen(pwd)); /* Write the current working directory to standard output */
		write(STDOUT_FILENO, "\n", 1); /* Write a newline character to standard output */
		free(pwd); /* Free the memory allocated for the current working directory string */
		return (0); /* Return 0 to indicate successful completion of the function */
	}
	else /* If pwd is NULL, indicating an error in getting the current directory */
	{
		if (errno == ENOENT) /* Check if the error is due to a non-existent directory */
			return (ft_path_error(ERR_NO_SUCH_FILE, tools, NULL)); /* Return an error for no such file */
		else if (errno == EACCES) /* Check if the error is due to permission denied */
			return (ft_path_error(ERR_PERMISSION_DENIED, tools, NULL)); /* Return an error for permission denied */
	}
	return (ft_path_error(ERR_NO_SUCH_FILE, tools, NULL)); /* Fallback error for no such file */
}
