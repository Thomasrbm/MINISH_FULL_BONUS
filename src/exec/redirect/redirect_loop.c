/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:43:41 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 14:41:53 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	redirect_exec_loop(t_shell *shell, int *wait_pid, int i, int j)
{
	int	exit_code;

	exit_code = 0;
	while (shell->pipe_count != -1)
	{
		if (shell->out_count != 0)
			shell->save_in = (STDIN_FILENO);
		shell->in_count = 0;
		shell->out_count = 0;
		shell->save_out = dup(STDOUT_FILENO);
		wait_pid[i] = process_redirect(shell, wait_pid, i);
		shell->cmd = next_pipe(shell->cmd);
		check_next_pipe(shell);
		shell->pipe_count--;
		i++;
		close(shell->save_out);
		close(shell->save_in);
	}
	while (j < i)
	{
		exit_code = wait_ignore(wait_pid[j]);
		j++;
	}
	return (exit_code);
}

void	redirect_loop(t_shell *shell)
{
	pid_t	*wait_pid;
	int		exit_code;
	int		i;
	int		j;
	int		doc_count;

	i = 0;
	j = 0;
	exit_code = 0;
	doc_count = do_here_doc(shell);
	shell->cmd = replace_doc(shell->cmd);
	shell->prev_fd = -1;
	wait_pid = malloc(sizeof(pid_t) * (shell->pipe_count + 1));
	shell->save_in = dup(STDIN_FILENO);
	exit_code = redirect_exec_loop(shell, wait_pid, i, j);
	if (shell->prev_fd > 0)
		close(shell->prev_fd);
	free(shell->indexes);
	free_all(shell, NULL);
	free(wait_pid);
	remove_tmp_doc(doc_count);
	exit(exit_code);
}

void	do_redirect(t_shell *shell, int pipefd[2])
{
	if (shell->prev_fd > 0 && shell->in_count == 0)
		dup2(shell->prev_fd, STDIN_FILENO);
	if (shell->prev_fd > 0)
		close (shell->prev_fd);
	if (shell->pipe_count != 0 && shell->out_count == 0)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	redirect_child(t_shell *shell, int pipefd[2], int *wait_ignore)
{
	char	**cmd;

	free(wait_ignore);
	do_redirect(shell, pipefd);
	close(shell->save_in);
	close(shell->save_out);
	cmd = get_flag(shell);
	if (!cmd)
	{
		free_all(shell, NULL);
		exit (0);
	}
	if (is_builtin(shell)->check == 0)
	{
		free_tab(shell->cmd);
		shell->cmd = cmd;
		exec_builtin(shell->cmd, shell);
		free_all(shell, NULL);
		exit(0);
	}
	binary_path(cmd, shell);
	exec_path(cmd, shell);
}
