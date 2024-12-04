/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:34:21 by pmolzer           #+#    #+#             */
/*   Updated: 2024/11/29 22:57:29 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_messages.h"
#include "lexer_parser.h"
#include "minishell.h"

/* 
 * Handles the processing of space tokens in the input string.
 * This function checks if there is a token between the current index
 * and the start index. If a token exists, it extracts the substring
 * from the input string and adds it to the lexer list as either a 
 * command (if is_cmd is true) or an argument (if is_cmd is false).
 * After processing, it updates the start index to the next character
 * after the space and increments the current index.
 */
void	handle_space_token(t_tools *tools, int *i, int *start, bool *is_cmd)
{
	char	*token;

	if (*i > *start)
	{
		token = ft_substr(tools->args, *start, *i - *start);
		if (*is_cmd)
			add_tk(&(tools->lexer_lst), make_tk(token, T_CMD));
		else
			add_tk(&(tools->lexer_lst), make_tk(token, T_ARG));
		*is_cmd = false;
	}
	*start = *i + 1;
	(*i)++;
}

/* 
 * Handles the processing of special tokens in the input string.
 * This function checks if there is a token between the current index
 * and the start index. If a token exists, it extracts the substring
 * and adds it to the lexer list as either a command or an argument.
 * It also determines if the current character and the next character
 * form a special token by calling ft_two_tk. If a special token is found,
 * it updates the current index and resets the start index. If not, it
 * processes the current character as a single token and updates the
 * is_cmd flag based on whether the last processed token was a pipe ('|').
 */
void	handle_special_token(t_tools *tools, int *i,
		int *start, bool *is_cmd)
{
	int		offset;
	char	*token;

	if (*i > *start)
	{
		token = ft_substr(tools->args, *start, *i - *start);
		if (*is_cmd)
			add_tk(&(tools->lexer_lst), make_tk(token, T_CMD));
		else
			add_tk(&(tools->lexer_lst), make_tk(token, T_ARG));
	}
	offset = ft_two_tk(tools->args[*i], tools->args[*i + 1], tools);
	if (offset > 0)
	{
		*i += offset;
		*start = *i;
		*is_cmd = false;
		return ;
	}
	ft_one_tk(tools->args[*i], tools);
	(*i)++;
	*start = *i;
	*is_cmd = (tools->args[*i - 1] == '|');
}

/* 
 * Core function for processing tokens in the input string.
 * This function iterates through the input string character by character,
 * checking for quote statuses and spaces. It delegates the handling of
 * tokens to the appropriate functions (handle_space_token and 
 * handle_special_token) based on the current character and quote status.
 * After processing all characters, it calls handle_final_token to 
 * handle any remaining token. The function returns 1 if processing 
 * completes without quotes, or 0 if there are unclosed quotes.
 */
int	handle_token_processing_core(t_tools *tools, int *i,
	char *quote_type, bool *is_cmd)
{
	int		start;

	start = 0;
	while (tools->args[*i])
	{
		if (check_quote_status(tools->args, i, quote_type))
			continue ;
		if (tools->args[*i] == ' ' && *quote_type == '\0')
		{
			handle_space_token(tools, i, &start, is_cmd);
			continue ;
		}
		if (*quote_type == '\0' && check_tk(tools->args[*i]))
		{
			handle_special_token(tools, i, &start, is_cmd);
			continue ;
		}
		(*i)++;
	}
	handle_final_token(tools, start, *i, *is_cmd);
	if (*quote_type == '\0')
		return (1);
	return (0);
}

/* 
 * Main function for handling token processing.
 * This function initializes the index to zero and sets the quote type
 * to an empty character. It then calls the core token processing function
 * (handle_token_processing_core) to process the input string and 
 * returns the result of that processing. This function serves as the 
 * entry point for token processing in the lexer.
 */
int	handle_token_processing(t_tools *tools, bool *is_cmd)
{
	int		i;
	char	quote_type;

	i = 0;
	quote_type = '\0';
	return (handle_token_processing_core(tools, &i, &quote_type, is_cmd));
}
