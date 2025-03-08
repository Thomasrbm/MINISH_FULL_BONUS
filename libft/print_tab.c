/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tab.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:07:23 by throbert          #+#    #+#             */
/*   Updated: 2025/03/06 16:18:49 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	printab(char *fmt, char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		printf("[%d] : ", i);
		printf(fmt, tab[i]);
		printf("\n");
		i++;
	}
}
