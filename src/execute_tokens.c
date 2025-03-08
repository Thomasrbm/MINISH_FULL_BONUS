/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:24:55 by throbert          #+#    #+#             */
/*   Updated: 2025/03/07 12:24:55 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	replace_double_operators2(char **toks, int *i)
{
	int	j;

	free(toks[*i]);
	free(toks[*i + 1]);
	toks[*i] = ft_strdup("&&");
	j = *i + 1;
	while (toks[j])
	{
		toks[j] = toks[j + 1];
		j++;
	}
	(*i)--;
}

// go throguh the toks array 
// if find | and | in i+1 then replace | with || and free the case in i+1
// same with &&
// it allow to avec || et && tout coller with the command like true&&echo d
// because parse cmd has split it all
static void	replace_double_operators(char **toks, int *i)
{
	int	j;

	if (toks[*i] && toks[*i + 1] && ft_strcmp(toks[*i], "|") == 0
		&& ft_strcmp(toks[*i + 1], "|") == 0)
	{
		free(toks[*i]);
		free(toks[*i + 1]);
		toks[*i] = ft_strdup("||");
		j = *i + 1;
		while (toks[j])
		{
			toks[j] = toks[j + 1];
			j++;
		}
		(*i)--;
	}
	else if (toks[*i] && toks[*i + 1] && ft_strcmp(toks[*i], "&") == 0
		&& ft_strcmp(toks[*i + 1], "&") == 0)
		replace_double_operators2(toks, i);
}

// If precedent is not 0 and its && then return 0 which will skip the current 
// command
// opposit for || 
// juste skip bc if then antother operator is valid it will still work even 
// if the 7
// precendent are skipped
// ls asdasd && ls && ls || ls  the || will work with the first ls  even 
//it ther others
// were skipped
static int	handle_logical_operators(char **toks, int i, t_shell *shell)
{
	if (ft_strcmp(toks[i], "&&") == 0)
	{
		if (shell->exit_code != 0)
			return (0);
	}
	else if (ft_strcmp(toks[i], "||") == 0)
	{
		if (shell->exit_code == 0)
			return (0);
		return (1);
	}
	return (1);
}

// end take i to always know where the last command was
// and search the end of the current command (till next control operator)
//
// end will copie every thing from i to end in shell->cmd to skip the cmd
//  alreayd done
// get wildcard if any
//
//  *i = end - 1;  in case we have echo sdad asd ads asd. to not stay on echo.
// allow to stay on last word before the operator 
//
// because before build cmd echo asad ads asd  is on 1 line 
// after it will be one word in each case
// of the tab
// [0] echo asad ads asd 
// 
// becomes 
// [0] : echo
// [1] : asad
// [2] : ads
// [3] : asd

// go throguh all tab and replace |  | by || and & & by &&
static void	preprocess_tokens(char **toks)
{
	int	i;

	i = 0;
	while (toks[i])
	{
		replace_double_operators(toks, &i);
		i++;
	}
}

// ececute every token in btw && and || operators
//
// if on operator handle it and skip it
// Exécuter chaque token entre les opérateurs && et ||
// Si c'est un opérateur, on le traite et on passe au suivant
void	execute_tokens(t_shell *shell, char **toks)
{
	int	i;
	int	exec_next;

	i = 0;
	exec_next = 1;
	preprocess_tokens(toks);
	while (toks[i])
	{
		if (ft_strcmp(toks[i], "&&") == 0 || ft_strcmp(toks[i], "||") == 0)
		{
			exec_next = handle_logical_operators(toks, i, shell);
			i++;
		}
		else
		{
			if (toks[i][0] == '(' && toks[i][ft_strlen(toks[i]) - 1] == ')')
				execute_subshell(toks[i], shell);
			else
				execute_command(shell, toks, &i, &exec_next);
			i++;
		}
	}
}
