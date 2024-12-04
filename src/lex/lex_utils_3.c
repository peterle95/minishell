/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:29:03 by pmolzer           #+#    #+#             */
/*   Updated: 2024/12/01 18:49:15 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_messages.h"
#include "lexer_parser.h"
#include "minishell.h"

/* char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
} */

int	check_tk(char tk)
{
	if (tk == '|' || tk == '<' || tk == '>')
		return (1);
	return (0);
}

/* 
 * This function handles the final token processing after parsing the input string.
 * It checks if there is a valid token between the start and current index,
 * and adds it to the lexer list as either a command or an argument based on the is_cmd flag.
 * This function is part of the lexer module, which is responsible for tokenizing input
 * before it is processed by the parser and executor in the minishell project.
 */
void	handle_final_token(t_tools *tools, int start, int i, bool is_cmd)
{
	char	*token;

	if (i > start) /* Check if the current index is greater than the start index */
	{
		token = ft_substr(tools->args, start, i - start); /* Extract the substring from args using start and current index */
		if (is_cmd) /* Check if the current token is a command */
			add_tk(&(tools->lexer_lst), make_tk(token, T_CMD)); /* Add the token as a command to the lexer list */
		else /* If it is not a command */
			add_tk(&(tools->lexer_lst), make_tk(token, T_ARG)); /* Add the token as an argument to the lexer list */
	}
}


/*
1.Initialization: The quote_type variable is used to track the current state of quotes. 
It is initialized to '\0', indicating that no quotes are currently open.
2. Opening Quotes:
When the function encounters a character that is either a single quote (') or a double quote ("), 
it checks if quote_type is '\0'.
If it is, this means there are no open quotes, and the function sets quote_type to the character 
of the quote found. This indicates that a quote has been opened.
The index i is incremented to move to the next character.
Closing Quotes:
If quote_type is not '\0', it means we are currently inside a quoted section.
The function checks if the current character matches the quote_type. If it does, 
this indicates that the quote is being closed.
The function then resets quote_type to '\0', indicating that there are no open 
quotes anymore, and increments the index i.
Return Values:
The function returns 1 when it successfully finds an opening or closing quote, 
indicating a change in quote status.
It returns 0 when no quote status change occurs, meaning the current character is not a quote.*/
int	check_quote_status(char *args, int *i, char *quote_type)
{
	// Check if the current quote type is not set (i.e., no open quotes)
	if (*quote_type == '\0')
	{
		// If the current character is a double quote or single quote
		if (args[*i] == '"' || args[*i] == '\'')
		{
			// Set the quote_type to the current quote character
			*quote_type = args[*i];
			// Move to the next character
			(*i)++;
			return (1); // Indicate that a quote was found and opened
		}
	}
	else
	{
		// If we are currently inside a quote (quote_type is set)
		// Check if the current character matches the open quote type
		if (args[*i] == *quote_type)
		{
			// If it matches, we found a closing quote
			*quote_type = '\0'; // Reset quote_type to indicate no open quotes
			(*i)++; // Move to the next character
			return (1); // Indicate that a closing quote was found
		}
	}
	return (0); // No quote status change
}
