/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 22:34:42 by pmolzer           #+#    #+#             */
/*   Updated: 2024/11/29 22:59:39 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Updates the PWD and OLDPWD environment variables after a directory change.
 * This function is part of the builtins module, which manages built-in commands
 * in the shell. It interacts with the environment variables to keep track of
 * the current working directory (PWD) and the previous working directory (OLDPWD).
 */
int	cd_update_env_paths(t_tools *tools) /* Function to update the PWD and OLDPWD environment variables */
{
	char	*pwd; /* Variable to store the current PWD value */
	char	*oldpwd_env; /* Variable to store the OLDPWD environment variable string */
	char	*newpwd; /* Variable to store the new PWD value obtained from getcwd */

	pwd = get_env_value("PWD", tools); /* Retrieve the current PWD from the environment variables */
	if (pwd) /* Check if the PWD was successfully retrieved */
	{
		oldpwd_env = ft_strjoin("OLDPWD=", pwd); /* Create a new string for OLDPWD by concatenating "OLDPWD=" with the current PWD */
		if (oldpwd_env) /* Check if the OLDPWD string was successfully created */
		{
			update_or_add_envp(&tools->envp, oldpwd_env); /* Update or add the OLDPWD variable in the environment */
			free(oldpwd_env); /* Free the memory allocated for the OLDPWD string */
		}
	}
	newpwd = getcwd(NULL, 0); /* Get the current working directory and store it in newpwd */
	if (newpwd) /* Check if the new PWD was successfully retrieved */
	{
		update_or_add_envp(&tools->envp, ft_strjoin("PWD=", newpwd)); /* Update the PWD variable in the environment */
		free(newpwd); /* Free the memory allocated for the new PWD string */
	}
	else /* If newpwd is NULL, indicating an error in getting the current directory */
	{
		update_or_add_envp(&tools->envp, ft_strjoin("PWD=", pwd)); /* Fallback to using the old PWD value */
	}
	return (0); /* Return 0 to indicate successful completion of the function */
}

/**
 * Changes the current directory, handling HOME, OLDPWD, and error conditions.
 * This function is part of the builtins module, which manages built-in commands
 * in the shell, and interacts with the environment variables to change the 
 * current working directory based on user input or defaults.
 */
int	ft_cd(char *path, t_tools *tools) /* Function to change the current directory */
{
	char	*home; /* Variable to store the HOME environment variable */
	char	*oldpwd; /* Variable to store the OLDPWD environment variable */

	home = get_env_value("HOME", tools); /* Retrieve the HOME environment variable */
	oldpwd = get_env_value("OLDPWD", tools); /* Retrieve the OLDPWD environment variable */
	if (!path) /* Check if no path is provided */
	{
		if (!home) /* If HOME is not set */
			return (ft_error(ERR_HOME_NOT_SET, tools)); /* Return an error indicating HOME is not set */
		path = home; /* Set path to HOME if no path is provided */
	}
	if (ft_strcmp(path, "-") == 0) /* Check if the path is "-" indicating to switch to OLDPWD */
	{
		if (!oldpwd) /* If OLDPWD is not set */
			return (ft_error(ERR_OLDPWD_NOT_SET, tools)); /* Return an error indicating OLDPWD is not set */
		path = oldpwd; /* Set path to OLDPWD */
	}
	if (chdir(path) != 0) /* Attempt to change the directory to the specified path */
	{
		if (errno == ENOENT) /* Check if the error is due to a non-existent directory */
			return (ft_path_error(ERR_NO_SUCH_FILE, tools, path)); /* Return an error for no such file */
		if (errno == EACCES) /* Check if the error is due to permission denied */
			return (ft_path_error(ERR_PERMISSION_DENIED, tools, path)); /* Return an error for permission denied */
	}
	return (cd_update_env_paths(tools)); /* Update the environment variables for PWD and OLDPWD */
}
