/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 22:38:42 by dmusulas          #+#    #+#             */
/*   Updated: 2024/11/28 16:30:02 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_messages.h"
#include "lexer_parser.h"

/*
 * This function checks for unclosed quotes in a given string.
 * It iterates through the string, toggling a quote type when it encounters
 * a quote character. If it finds an unmatched quote at the end, it reports
 * an error and returns 0; otherwise, it returns 1 indicating all quotes are closed.
 *
 * Step-by-step explanation:
 * 1. Initialize an index `i` to 0 and a variable `quote_type` to 0.
 * 2. Loop through each character in the string `s` until the end.
 * 3. If `quote_type` is 0 and the current character is a quote (either ' or "),
 *    set `quote_type` to the current character.
 *    Example: For the string `"Hello 'world'"`, when `i` is at the first `"`, 
 *    `quote_type` becomes `'"'`.
 * 4. If the current character matches `quote_type`, reset `quote_type` to 0.
 *    Example: When `i` reaches the second `"`, `quote_type` resets to 0.
 * 5. After the loop, if `quote_type` is still set, it means there is an unmatched quote.
 * 6. Call `ft_error` to report the error and return 0.
 * 7. If all quotes are matched, return 1.
 */
int	check_quotes(char *s, t_tools *tools)
{
	int		i; // Initialize index variable
	char	quote_type; // Variable to track the current quote type

	i = 0; // Start at the beginning of the string
	quote_type = 0; // No quote type is set initially
	while (s[i]) // Loop through each character in the string
	{
		if (!quote_type && (s[i] == '"' || s[i] == '\'')) // Check for opening quote
			quote_type = s[i]; // Set quote_type to the current quote
		else if (s[i] == quote_type) // Check for closing quote
			quote_type = 0; // Reset quote_type if closing quote is found
		i++; // Move to the next character
	}
	if (quote_type) // After the loop, check if there is an unmatched quote
	{
		ft_error(ERR_QUO, tools); // Report the error for unmatched quote
		return (0); // Return 0 indicating an error
	}
	return (1); // Return 1 indicating all quotes are closed
}
