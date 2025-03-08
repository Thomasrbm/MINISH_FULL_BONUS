/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 05:51:59 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/08 00:09:54 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	error_open(t_shell *shell, int i, int message)
{
	if (message == 1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(shell->cmd[i + 1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	else
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(shell->cmd[i + 1], 2);
		ft_putstr_fd(": permission denied\n", 2);
		return (1);
	}
}

void	free_all(t_shell *shell, char **cmd_files)
{
	if (cmd_files)
		free_tab(cmd_files);
	if (shell->cmd)
		free_tab(shell->cmd);
	if (shell->env)
		free_tab(shell->env);
	if (shell->env_for_sub)
		free_tab(shell->env_for_sub);
	if (shell)
		free(shell);
}

void	binary_path(char **cmd, t_shell *shell)
{
	if (cmd[0] == NULL)
	{
		free_all(shell, NULL);
		exit(127);
	}
	if (access(cmd[0], X_OK) == 0)
	{
		if (execve(cmd[0], cmd, shell->env) == -1)
		{
			ft_putstr_fd("execve failed\n", 2);
			free_all(shell, cmd);
			exit(127);
		}
	}
}

void	exit_no_path(t_shell *shell, char **cmd, char **cmd_files)
{
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_all(shell, cmd);
	if (cmd_files)
		free_tab(cmd_files);
	exit(127);
}

void	exec_path(char **cmd, t_shell *shell)
{
	int		i;
	char	**cmd_files;

	i = 0;
	cmd_files = split_path(cmd, shell->env);
	if (!cmd_files || !cmd_files[0])
		exit_no_path(shell, cmd, cmd_files);
	while (cmd_files[i] != NULL)
	{
		if (access(cmd_files[i], X_OK) == 0)
		{
			if (execve(cmd_files[i], cmd, shell->env) == -1)
			{
				free_tab(cmd);
				free_all(shell, cmd_files);
				exit(127);
			}
		}
		i++;
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_tab(cmd);
	free_all(shell, cmd_files);
	exit(127);
}
