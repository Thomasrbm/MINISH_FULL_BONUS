/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 06:33:46 by rbardet-          #+#    #+#             */
/*   Updated: 2025/02/27 21:30:46 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**replace_doc(char **cmd)
{
	int		i;
	int		doc_count;
	char	*new_token;
	char	*num_str;

	i = 0;
	doc_count = 1;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], "<<") == 0)
		{
			free(cmd[i]);
			cmd[i] = ft_strdup("<");
			num_str = ft_itoa(doc_count++);
			new_token = ft_strjoin(".heredoc", num_str);
			free(num_str);
			free(cmd[i + 1]);
			cmd[i + 1] = new_token;
			i += 2;
		}
		else
			i++;
	}
	return (cmd);
}

void	remove_tmp_doc(int doc_count)
{
	char	*file;
	char	*tmp2;

	while (doc_count != 0)
	{
		tmp2 = ft_itoa(doc_count);
		file = ft_strjoin(".heredoc", tmp2);
		unlink(file);
		doc_count--;
		free(tmp2);
		free(file);
	}
}

int	do_here_doc(t_shell *shell)
{
	int	i;
	int	doc_count;

	i = 0;
	doc_count = 0;
	while (shell->cmd[i] != NULL)
	{
		if (!ft_strcmp(shell->cmd[i], "<<"))
		{
			doc_count++;
			if ((handle_doc(shell, i, doc_count)) == 130)
			{
				free(shell->indexes);
				remove_tmp_doc(doc_count);
				free_all(shell, NULL);
				exit (130);
			}
		}
		i++;
	}
	return (doc_count);
}
