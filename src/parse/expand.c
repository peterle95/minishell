/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 22:24:49 by dmusulas          #+#    #+#             */
/*   Updated: 2024/12/02 00:09:23 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_parser.h"
#include "minishell.h"

// This function retrieves the value of an environment variable from the tools structure.
// It takes the name of the variable (var_name) and searches through the linked list of environment variables (tools->envp).
// For example, if var_name is "HOME", the function will look for a string in the format "HOME=/path/to/home".
// If found, it returns the value after the '=' character, which in this case would be "/path/to/home".

char	*get_env_value(const char *var_name, t_tools *tools)
{
	t_list	*env_var;
	size_t	var_len;
	char	*env_str;

	env_var = tools->envp;
	var_len = ft_strlen(var_name);
	// Iterate through the linked list of environment variables
	while (env_var)
	{
		// Cast the content of the current node to a string
		env_str = (char *)env_var->content;
		// Compare the beginning of the environment string with the variable name
		// Check if the variable name matches and is followed by an '=' character
		if (ft_strncmp(env_str, var_name, var_len) == 0
			&& env_str[var_len] == '=')
		{
			// Return the value of the environment variable, which is the part after '='
			return (env_str + var_len + 1);
		}
		// Move to the next environment variable in the list
		env_var = env_var->next;
	}
	return (NULL);
}

static char	*append_char(char *str, char c)
{
	char	*temp_str;
	size_t	len;

	if (str)
		len = ft_strlen(str);
	else
		len = 0;
	temp_str = malloc(len + 2);
	if (!temp_str)
	{
		free(str);
		return (NULL);
	}
	if (str)
	{
		ft_strlcpy(temp_str, str, len + 1);
		free(str);
	}
	if (len + 1 < len + 2)
		temp_str[len] = c;
	temp_str[len + 1] = '\0';
	return (temp_str);
}

static char	*expand_single_var(const char *s, size_t *i, t_tools *tools)
{
	size_t	var_start;
	size_t	var_end;
	char	*var_name;
	char	*var_value;

	var_start = *i + 1;
	var_end = var_start;
	while (s[var_end] && (ft_isalnum(s[var_end]) || s[var_end] == '_'))
		var_end++;
	var_name = ft_substr(s, var_start, var_end - var_start);
	var_value = get_env_value(var_name, tools);
	free(var_name);
	*i = var_end - 1;
	if (var_value)
		return (ft_strdup(var_value));
	else
		return (ft_strdup(""));
}

char	*expand_var(const char *s, t_tools *tools)
{
	char	*result;
	char	*temp;
	char	*new_result;
	size_t	i;

	result = ft_strdup("");
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && (ft_isalpha(s[i + 1]) || s[i
					+ 1] == '_'))
			temp = expand_single_var(s, &i, tools);
		else
			temp = append_char(ft_strdup(""), s[i]);
		new_result = ft_strjoin(result, temp);
		free(result);
		free(temp);
		result = new_result;
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
