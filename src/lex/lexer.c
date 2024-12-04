/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:01:05 by dmusulas          #+#    #+#             */
/*   Updated: 2024/12/01 18:50:16 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*The tokenization process in the provided code is a crucial part of parsing 
input strings in a shell-like environment. It involves breaking down a string
of commands into manageable pieces called tokens, which can be processed further.
Below is a detailed explanation of the tokenization process, covering the relevant
functions and their roles.

### Overview of Tokenization

1. **Input String**: The process starts with an input string that may contain 
commands, arguments, and special characters (like pipes, redirection symbols, etc.).
2. **Tokens**: Tokens are the individual components extracted from the input 
	string. They can be commands, arguments, or special characters.
3. **Handling Quotes**: The tokenizer must correctly handle quoted strings 
to ensure that spaces within quotes are treated as part of the token rather than as delimiters.

### Key Functions and Their Roles

#### 1. `tokenize_input(t_tools *tools)`

- **Purpose**: This function initiates the tokenization process.
- **Process**:
  - It sets a flag `is_cmd` to `true`, indicating that the first token is expected to be a command.
  - It calls `handle_token_processing(tools, &is_cmd)` to process the input string and generate tokens.
  - If the result of token processing is zero, it indicates a syntax error, and an error message is returned.

#### 2. `handle_token_processing(t_tools *tools, bool *is_cmd)`

- **Purpose**: This function processes the input string character by character.
- **Process**:
  - It initializes an index `i` to track the current position in the input 
  			string and a `quote_type` variable to manage quote states.
  - It calls `handle_token_processing_core(tools, &i, &quote_type, is_cmd)` 
  			to perform the core tokenization logic.

#### 3. `handle_token_processing_core(t_tools *tools, int *i, char *quote_type, bool *is_cmd)`

- **Purpose**: This function contains the main loop for processing the input string.
- **Process**:
  - It iterates through the input string using the index `i`.
  - It checks for quote status using `check_quote_status(tools->args, i, quote_type)`. 
  	If a quote is found, it updates the `quote_type` and continues.
  - If a space is encountered and no quotes are open, it calls `handle_space_token` to finalize the current token.
  - If a special token (like `|`, `<`, `>`) is found and no quotes are open, it calls `handle_special_token`.
  - After processing all characters, it calls `handle_final_token` to finalize any remaining token.

#### 4. `handle_space_token(t_tools *tools, int *i, int *start, bool *is_cmd)`

- **Purpose**: This function handles the creation of tokens when a space is encountered.
- **Process**:
  - If the current index `i` is greater than `start`, it means there is a token to process.
  - It extracts the substring from `start` to `i` and determines if it is a command or an 
  	argument based on the `is_cmd` flag.
  - It adds the token to the lexer list and updates the `start` index for the next token.

#### 5. `handle_special_token(t_tools *tools, int *i, int *start, bool *is_cmd)`

- **Purpose**: This function handles special tokens like pipes and redirection.
- **Process**:
  - Similar to `handle_space_token`, it checks if there is a token to process.
  - It uses `ft_two_tk` to check for two-character tokens (like `<<` or `>>`). If found, 
  	it updates the index `i` and resets the `start` index.
  - If no two-character token is found, it calls `ft_one_tk` to handle single-character tokens.

#### 6. `check_quote_status(char *args, int *i, char *quote_type)`

- **Purpose**: This function checks the status of quotes in the input string.
- **Process**:
  - It toggles the `quote_type` variable when encountering quote characters. 
  	If a quote is opened, it sets `quote_type` to that character; if closed, it resets `quote_type` to zero.
  - This ensures that tokens within quotes are treated as single tokens, even if they contain spaces.

#### 7. `check_tk(char tk)`

- **Purpose**: This function checks if a character is a special token.
- **Process**: It returns `1` if the character is a pipe (`|`), input redirection (`<`), 
	or output redirection (`>`), and `0` otherwise.

#### 8. `add_tk(t_lexer **lexer_list, t_lexer *new_token)`

- **Purpose**: This function adds a new token to the lexer list.
- **Process**: It traverses the linked list of tokens and appends the new token at the end.

#### 9. `make_tk(char *str, t_tokens token)`

- **Purpose**: This function creates a new token structure.
- **Process**: It allocates memory for a new token, initializes its fields, and returns the pointer to the new token.

### Summary of the Tokenization Process

1. **Initialization**: The process begins with the `tokenize_input` function, 
	which sets up the necessary structures and flags.
2. **Character Processing**: The input string is processed character by character, 
	checking for quotes, spaces, and special tokens.
3. **Token Creation**: When a space or special token is encountered, the current 
	substring is extracted and added to the token list.
4. **Finalization**: After processing all characters, any remaining tokens are 
	finalized and added to the list.
5. **Error Handling**: Throughout the process, checks for unmatched quotes and 
	memory allocation failures are performed, ensuring robust error handling.

This detailed breakdown illustrates how the tokenization process works in the 
provided code, highlighting the importance of handling quotes and special 
characters correctly to ensure accurate parsing of shell commands.*/

/* 
 * This function tokenizes the input for the lexer by processing the input string 
 * and generating tokens based on the provided tools structure. It initializes 
 * the lexer list and checks if the first token is a command. If the tokenization 
 * fails due to a syntax error, it returns an error code. This function is crucial 
 * for the overall parsing process in the shell, as it prepares the tokens for 
 * further processing in the parser.
 */
int	tokenize_input(t_tools *tools) /* Function declaration for tokenizing input */
{
	bool	is_cmd; /* Boolean flag to indicate if the current token is a command */
	int		result; /* Variable to store the result of the token processing */

	is_cmd = true; /* Initialize is_cmd to true, indicating the first token is expected to be a command */
	tools->lexer_lst = NULL; /* Set the lexer list in tools to NULL to start fresh */
	result = handle_token_processing(tools, &is_cmd); /* Call the token processing function and store the result */
	if (!result) /* Check if the result indicates a failure */
		return (ft_error(ERR_SYNTAX, tools)); /* Return a syntax error if tokenization failed */
	return (1); /* Return 1 to indicate successful tokenization */
}
