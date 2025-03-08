/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:17:13 by throbert          #+#    #+#             */
/*   Updated: 2025/03/07 12:17:13 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// because we get (    )  subshell cmd in 1 char * 
// we remove parenthese on 1 char * 
// and realloc
char	*remove_parentheses(char *toks)
{
	int		len;
	char	*new_toks;

	if (!toks)
		return (NULL);
	new_toks = NULL;
	len = strlen(toks);
	if (len > 1 && toks[0] == '(' && toks[len - 1] == ')')
	{
		new_toks = strndup(toks + 1, len - 2);
		return (new_toks);
	}
	return (toks);
}

// ft split like in process input line but goes through our parsing
// like a real prompt
// no need to go again through validate input because we already did it before
char	**parse_subshell(char *toks, t_shell *sub_shell)
{
	char	**subshell_toks;
	char	*parsed_trim;

	parsed_trim = remove_parentheses(toks);
	if (!parsed_trim)
		return (NULL);
	subshell_toks = ft_split_token(parsed_trim, sub_shell);
	free(parsed_trim);
	return (subshell_toks);
}

// init subshell same as shell
void	init_subshell(t_shell *sub_shell)
{
	sub_shell->cmd = NULL;
	sub_shell->env_for_sub = NULL;
	sub_shell->exit_code = 0;
	sub_shell->indexes = 0;
	sub_shell->in_single_quote = 0;
	sub_shell->in_double_quote = 0;
	sub_shell->pipe_count = 0;
	sub_shell->check = 0;
	sub_shell->save_in = 0;
	sub_shell->size = 0;
	sub_shell->save_out = 0;
	sub_shell->in_count = 0;
	sub_shell->out_count = 0;
}

// we fork a children process that will init his shell
// 
// execute token in his env
//
// the parent will wait for the children to finish
// and get the exit code
void	sub_fork(t_shell *sub_shell, char *toks,
		char **sub_commands, t_shell *shell)
{
	pid_t	pid;
	int		status;

	sub_commands = parse_subshell(toks, sub_shell);
	if (sub_commands == NULL)
		return ;
	pid = fork();
	if (pid == 0)
	{
		init_subshell(sub_shell);
		execute_tokens(sub_shell, sub_commands);
		free_tab(sub_commands);
		exit(sub_shell->exit_code);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		shell->exit_code = WEXITSTATUS(status);
		free_tab(sub_commands);
	}
}

// create new shell only for exec this subshell
// and runs it in a fork above
void	execute_subshell(char *toks, t_shell *shell)
{
	t_shell	*sub_shell;
	char	**sub_commands;

	sub_shell = malloc(sizeof(t_shell));
	if (!sub_shell)
		return ;
	sub_shell->env = ft_tabdup(shell->env_for_sub);
	sub_commands = NULL;
	sub_fork(sub_shell, toks, sub_commands, shell);
	if (sub_commands != NULL)
		free_tab(sub_commands);
	if (sub_shell->env != NULL)
		free_tab(sub_shell->env);
	free(sub_shell);
}

//  (export USER=d) && echo $USER

//  (cd /tmp && ls)

// (mkdir testdir && cd testdir && touch file.txt) || echo "Failed" => print rien

// (echo "Creating directory" && mkdir new_dir && cd new_dir && touch new_file.txt && echo "File created")

// (cd /tmp && ls) || echo "Failed to list files" ==> print rien

// (mkdir test_dir && cd test_dir && touch test_file.txt) || echo "Something went wrong"  => print rien

// (echo "Starting process" && mkdir test && cd test && touch file1.txt && ls) && echo "Process completed"  => print 



// PBBBBBBBBBB :  (cat) ||  echo $? avec ctrl + z et ctrl + backslah print pas le bon