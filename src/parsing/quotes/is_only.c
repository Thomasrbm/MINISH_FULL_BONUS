/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_only.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 18:05:16 by throbert          #+#    #+#             */
/*   Updated: 2025/03/07 19:07:13 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check is pair number os same quote till find another type
// is call again in proccess for the next group
// has int pointer to keep track of the index
// 
// check if quote is pair else returm error
int	check_quotes_group(char *s, int *i)
{
	char	cur;
	int		cnt;

	cur = s[*i];
	cnt = 0;
	while (s[*i] && s[*i] == cur)
	{
		cnt++;
		(*i)++;
	}
	return (cnt % 2 == 0);
}

// go through str and check if juste quote
// if other put pointer int to 1
// is_only will recup it and say error if other = 1
static int	process_quotes(char *s, int *i, int *other, int *quote)
{
	while (s[*i] && !ft_isspace(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '\"')
		{
			*quote = 1;
			if (!check_quotes_group(s, i))
				return (0);
		}
		else
		{
			*other = 1;
			(*i)++;
		}
	}
	return (1);
}

// if more than 1 quote check if is only quotes
int	is_only_quotes(char *s)
{
	int	i;
	int	other;
	int	quote;
	int	len;

	i = 0;
	other = 0;
	quote = 0;
	len = ft_strlen(s);
	if (len == 2 && ((s[0] == '\"' && s[1] == '\"')
			|| (s[0] == '\'' && s[1] == '\'')))
		return (1);
	if (!process_quotes(s, &i, &other, &quote))
		return (0);
	return ((!other) && quote);
}

// fc = first char
// will check if only backslashes
// or only `
// if \ and ` return error
// if other char return error
// 
// if pair length return good
// non pair = error
int	is_only_backslashes_or_backtick(char *s)
{
	int		i;
	char	fc;

	if (!s || !*s)
		return (0);
	fc = s[0];
	if (fc != '\\' && fc != '`')
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] != fc)
			return (0);
		i++;
	}
	if (i % 2 == 0)
		return (0);
	return (1);
}
