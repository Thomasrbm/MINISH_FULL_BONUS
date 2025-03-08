/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_closed2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:12:52 by throbert          #+#    #+#             */
/*   Updated: 2025/03/07 12:12:52 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// will put 1 if find a char that open a quote Or STAY 0
// backslash escpae also in normal str
int	handle_normal(char *s, int i, int state[4])
{
	if (s[i] == '\\')
		state[3] = 1;
	else if (s[i] == '\'')
		state[0] = 1;
	else if (s[i] == '"')
		state[1] = 1;
	return (i);
}

// else if (s[i] == '$' && s[i + 1] == '\'')
// {
// 	state[2] = 1;
// 	i += 2;
// 	i--;
// }