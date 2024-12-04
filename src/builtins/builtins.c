/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 22:26:53 by pmolzer           #+#    #+#             */
/*   Updated: 2024/12/01 18:44:48 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_parser.h"
#include "minishell.h"

/* 
 * This function executes the 'cd' (change directory) command in the shell.
 * It takes a command node from the abstract syntax tree (AST) and a tools 
 * structure that contains various tools and state information for the shell.
 * The function checks for the presence of arguments and calls the appropriate 
 * function to change the directory, handling errors if too many arguments 
 * are provided. This function is part of the builtins module, which handles 
 * built-in commands in the shell, and interacts with the lexer and parser 
 * components to process user input.
 */
int	execute_cd(t_ast *cmd_node, t_tools *tools) /* Function to execute the 'cd' command */
{
	t_ast	*arg_node; /* Pointer to the argument node in the AST */
	int		status_result; /* Variable to store the result status of the command */

	status_result = 0; /* Initialize the status result to 0 */
	arg_node = cmd_node->right; /* Get the right child of the command node, which contains arguments */
	if (arg_node && arg_node->right) /* Check if there are more than one argument */
	{
		ft_error(ERR_TOO_MANY_ARGS, tools); /* Report an error for too many arguments */
		return (1); /* Return 1 to indicate an error occurred */
	}
	if (arg_node) /* If there is an argument */
		status_result = ft_cd(arg_node->str, tools); /* Call the change directory function with the argument */
	else /* If there are no arguments */
		status_result = ft_cd(NULL, tools); /* Call the change directory function with NULL to go to the home directory */
	return (status_result); /* Return the status result of the cd command */
}

void	execute_builtin(t_ast *cmd_node, t_tools *tools)
{
	int	status_result;

	status_result = 0;
	if (ft_strcmp(cmd_node->str, "echo") == 0)
		status_result = ft_echo(cmd_node, tools);
	else if (ft_strcmp(cmd_node->str, "cd") == 0)
		status_result = execute_cd(cmd_node, tools);
	else if (ft_strcmp(cmd_node->str, "pwd") == 0)
		status_result = ft_pwd(tools);
	else if (ft_strcmp(cmd_node->str, "export") == 0)
		status_result = ft_export(cmd_node->right, tools);
	else if (ft_strcmp(cmd_node->str, "unset") == 0)
		status_result = ft_unset(cmd_node->right, tools);
	else if (ft_strcmp(cmd_node->str, "env") == 0)
		status_result = ft_env(tools);
	else if (ft_strcmp(cmd_node->str, "exit") == 0)
		status_result = ft_exit(cmd_node, tools);
	tools->last_exit_status = status_result;
}
