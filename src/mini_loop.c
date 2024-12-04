/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 22:22:53 by dmusulas          #+#    #+#             */
/*   Updated: 2024/12/01 18:43:57 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "lexer_parser.h"
#include "minishell.h"
#include <stdlib.h>

void	init_tools(t_tools *tools)
{
	tools->args = NULL;
	tools->lexer_lst = NULL;
	tools->p_redir = NULL;
	tools->heredoc = false;
	tools->in_fd = 0;
	tools->out_fd = 0;
	tools->last_pid = 0;
	tools->tree = NULL;
	tools->in_single_quotes = false;
	init_signals();
}

void	set_initial_exit_status(t_tools *tools)
{
	tools->last_exit_status = 0;
}

int	reset_tools(t_tools *tools)
{
	if (tools)
		clean_tools(tools);
	init_tools(tools);
	mini_loop(tools);
	return (0);
}

int	mini_loop(t_tools *tools)
{
	tools->args = readline("minishell$ ");
	if (tools->args == NULL)
	{
		ft_putendl_fd("minishell$ exit", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	if (!tools->args || tools->args[0] == '\0')
		return (reset_tools(tools));
	add_history(tools->args);
	handle_input(tools);
	execute_command(tools->tree, tools);
	reset_tools(tools);
	return (1);
}
