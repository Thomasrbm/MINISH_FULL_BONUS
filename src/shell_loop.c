/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 00:50:53 by throbert          #+#    #+#             */
/*   Updated: 2025/03/08 03:07:27 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cycle(t_shell *shell);

// count because it will evovle btw each malloc
void	build_cmd(t_shell *shell, char **toks, int count)
{
	int	i;

	i = 0;
	shell->cmd = malloc(sizeof(char *) * (count + 1));
	if (!shell->cmd)
		return ;
	while (i < count)
	{
		shell->cmd[i] = ft_strdup(toks[i]);
		i++;
	}
	shell->cmd[i] = NULL;
}

void	execute_single_command(t_shell *shell)
{
	if (shell->cmd && shell->cmd[0])
	{
		if (is_redirection(shell) == 0)
			shell->exit_code = redirect(shell);
		else if (is_builtin(shell)->check == 0)
			shell->exit_code = exec_builtin(shell->cmd, shell);
		else if (check_single_cmd(shell, shell->cmd)
			&& !ft_tabchr(shell->cmd, '*'))
		{
			error_message(shell->cmd);
			shell->exit_code = 127;
		}
		else
			shell->exit_code = fork_and_exec(shell);
	}
}

void	execute_command(t_shell *shell, char **toks, int *i, int *exec_next)
{
	int	end;

	if (*exec_next)
	{
		end = *i;
		while (toks[end] && !(ft_strcmp(toks[end], "&&") == 0
				|| ft_strcmp(toks[end], "||") == 0))
			end++;
		build_cmd(shell, &toks[*i], end - *i);
		shell->cmd = expand_wildcard(shell->cmd, shell);
		execute_single_command(shell);
		free_tab(shell->cmd);
		*exec_next = (shell->exit_code == 0);
		*i = end - 1;
	}
}

int	handle_cycle(t_shell *shell)
{
	char	**toks;

	shell->in_single_quote = 0;
	shell->in_double_quote = 0;
	shell->pipe_count = 0;
	shell->check = 0;
	shell->save_in = 0;
	shell->size = 0;
	shell->save_out = 0;
	shell->in_count = 0;
	shell->out_count = 0;
	toks = process_input_line(shell);
	if (!toks)
		return (-1);
	execute_tokens(shell, toks);
	if (toks)
		free_tab(toks);
	return (0);
}

int	shell_loop(t_shell *shell)
{
	int	cycle_status;

	shell->exit_code = 0;
	shell->indexes = 0;
	while (1)
	{
		cycle_status = handle_cycle(shell);
		if (cycle_status == -1)
			break ;
	}
	clear_history();
	free(shell->indexes);
	free_tab(shell->env);
	free_tab(shell->env_for_sub);
	cycle_status = shell->exit_code;
	free(shell);
	return (cycle_status);
}
