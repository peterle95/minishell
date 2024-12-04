/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmusulas <dmusulas@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:01:28 by dmusulas          #+#    #+#             */
/*   Updated: 2024/11/12 18:44:49 by dmusulas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error_messages.h"
#include "lexer_parser.h"
#include "libft.h"

/*
 * This function calculates the length of the initial segment of string `s`
 * that contains no characters from string `reject`. It checks each character
 * in `s` to see if it appears in `reject`. If a match is found, it returns
 * the index of that character in `s`. If no match is found, it returns the
 * length of `s`. This function helps tokenize input strings in a shell parser
 * by identifying the first character that is not part of the current token.
 */
size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (reject[j])
		{
			if (s[i] == reject[j])
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

t_lexer	*make_tk(char *str, t_tokens token)
{
	t_lexer	*new_token;

	new_token = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_token)
	{
		perror("malloc");
		return (NULL);
	}
	new_token->str = str;
	new_token->token = token;
	new_token->next = NULL;
	return (new_token);
}

void	add_tk(t_lexer **lexer_list, t_lexer *new_token)
{
	t_lexer	*curr;

	curr = *lexer_list;
	if (!*lexer_list)
	{
		*lexer_list = new_token;
		return ;
	}
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new_token;
}

int	ft_one_tk(char c, t_tools *tools)
{
	if (c == '|')
	{
		add_tk(&(tools->lexer_lst), make_tk(ft_strdup("|"), T_PIPE));
	}
	else if (c == '<')
		add_tk(&(tools->lexer_lst), make_tk(ft_strdup("<"), T_REDIR_IN));
	else if (c == '>')
		add_tk(&(tools->lexer_lst), make_tk(ft_strdup(">"), T_REDIR_OUT));
	return (1);
}

int	ft_two_tk(char c1, char c2, t_tools *tools)
{
	if (c1 == '<' && c2 == '<')
	{
		add_tk(&(tools->lexer_lst), make_tk(ft_strdup("<<"), T_HEREDOC));
		return (2);
	}
	else if (c1 == '>' && c2 == '>')
	{
		add_tk(&(tools->lexer_lst), make_tk(ft_strdup(">>"), T_APPEND));
		return (2);
	}
	return (0);
}
