/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 07:40:00 by throbert          #+#    #+#             */
/*   Updated: 2025/03/08 00:47:05 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile __sig_atomic_t	g_signal;

/* Structures */

typedef struct t_shell
{
	int							exit_code;
	int							check;
	int							save_out;
	int							save_in;
	int							size;
	int							out_count;
	int							in_count;
	int							pipe_count;
	char						**env;
	char						**env_for_sub;
	char						**cmd;
	int							prev_fd;
	int							*indexes;
	int							in_single_quote;
	int							in_double_quote;
}								t_shell;

typedef struct t_doc
{
	char						*str;
	char						*limiter;
}								t_doc;

char							*append_str(char *s, char *to_add);
char							*append_char(char *s, char c);

/* Prototypes */

int								do_here_doc(t_shell *shell);
char							**replace_doc(char **cmd);
void							remove_tmp_doc(int doc_count);

/* Signal management */
void							setup_signals(int mode);
void							setup_signals2(int mode);
int								wait_ignore(pid_t pid);
void							sig_handler(int signum);
void							sigint_handler_heredoc(int sig);

/* Readline */
char							*get_readline(t_shell *shell);

/* Builtins */
int								ft_pwd(t_shell *shell);
int								ft_env(t_shell *shell);
char							**edit_env_value(t_shell *shell, char *value,
									char *new_one);
t_shell							*ft_unset(t_shell *shell);
char							**get_unset_value(t_shell *shell);
t_shell							*ft_export(t_shell *shell);
t_shell							*ft_cd(t_shell *shell);
t_shell							*ft_echo(char **cmd, t_shell *shell);

/* Redirections */
void							redirect_child(t_shell *shell, int pipefd[2],
									int *wait_ignore);
void							redirect_loop(t_shell *shell);
int								count_pipe(char **cmd);
int								find_command_index(char **cmd);
int								is_delim_token(char *token);
char							**get_flag(t_shell *shell);
int								update_count(char *line, int *j, int *count,
									char *current);
int								check_line_delim_chain(char *line);
int								is_delim(char *s);
int								calc_len_line(char *line);
int								mal_len(char **tab);
void							append_delim(char *res, int *r, char *token,
									int token_len);
void							close_all(char *str, char *limiter, int fd,
									t_shell *shell);

pid_t							process_cmd(t_shell *shell, int *wait_pid);
char							*find_env(char **env);
char							**split_path(char **cmd, char **env);
int								valid_cmd(char **cmd, char **env);
void							free_all(t_shell *shell, char **cmd_files);
void							exec_path(char **cmd, t_shell *shell);
void							binary_path(char **cmd, t_shell *shell);
int								process_redirect(t_shell *shell, int *wait_pid,
									int j);
int								handle_doc(t_shell *shell, int i,
									int doc_count);
int								error_open(t_shell *shell, int i, int message);
int								open_file(t_shell *shell, int i, int check);
int								open_file_2(t_shell *shell, int i, int check);
int								redirect_output(t_shell *shell, int i);
int								is_last_input(t_shell *shell, int i);
int								redirect_input(t_shell *shell, int i);
int								redirect(t_shell *shell);
int								is_ambiguous(t_shell *shell, int i);

/* Error management */
void							error_message(char **cmd);
int								check_single_cmd(t_shell *shell, char **cmd);

/* More builtins */
int								is_redirection(t_shell *s);
t_shell							*is_builtin(t_shell *s);
long long						ft_exit(t_shell *shell);
long long						exit_limits(t_shell *shell, int i,
									long long status);
void							free_exit(t_shell *shell);
long long						ft_atol_safe(char *str, int *error);

/* Environment variables */
char							*get_env_value(char *var, t_shell shell);
char							*get_env(t_shell *shell, char *value);
/* edit_env_value already declared above */
char							**split_env_path(char **cmd, char **env);

int								absolute_cmd(char **cmd, t_shell *shell);
char							*ft_strjoin_free_both(char *s1, char *s2);

/* AST */
void							get_sigint(t_shell *shell);

/* Commands and execution */
int								check_first(char *argv, char **env);

/* Shell loop */
int								shell_loop(t_shell *shell);

/* Signal management (again) */
int								wait_ignore(pid_t pid);

/* Wildcards expansion */

char							*remove_quotes_wild(char *s);

int								check_middle_tokens(char *dir_name,
									char **subtokens, int cnt, int *pos);
int								count_tokens_wild(char **tokens);
int								check_prefix(char *dir_name, char *token,
									int *pos);
void							detect_ambiguous_redirects(char **token_cmd,
									t_shell *shell);
char							**expand_token(char *token);
void							handle_dollar(char *in, int *i, char **w,
									t_shell *shell);

/* Readline helper */
int								ft_isspace(char c);

/* Additional redirection functions */
void							quote(char *res, char *line, int *i, int *r);
void							append_delim(char *res, int *r, char *token,
									int token_len);
void							check_next_pipe(t_shell *shell);

char							**process_input_line(t_shell *shell);
int								is_redirect(char *cmd);
char							**next_pipe(char **cmd);
int								exec_builtin(char **cmd, t_shell *shell);
int								err_semicolon(void);
int								err_ampersand(char *s);
int								err_curly(void);
char							*parse_cmd(char *line);
char							*ft_parse_word(char *in, int *pos,
									t_shell *shell);

void							handle_dollar(char *in, int *i, char **w,
									t_shell *shell);
void							handle_dollar_var(char *in, int *i, char **w,
									t_shell *shell);
void							handle_dollar_question(int *i, char **w,
									t_shell *shell);
char							**ft_split_token(char *in, t_shell *shell);
int								all_quotes_closed(char *s);
int								is_only_quotes(char *s);
int								err_backslash(void);
int								err_backtick(void);
char							*validate_input(char *in, t_shell *shell);
int								fork_and_exec(t_shell *shell);
int								exec(char **cmd, t_shell *shell);

char							*validate_input(char *in, t_shell *shell);

void							process_single_quote(const char *s, char *clean,
									int *i, int *j);

void							process_double_quote(const char *s, char *clean,
									int *i, int *j);

char							*ft_strjoin_free(char *s1, char *s2);

void							get_sigint(t_shell *shell);

void							not_valid_cmd(t_shell *shell, char **cmd);

int								match_pattern(char *dir, char *flag);

int								contain_wildcard(char *current_token);

char							**expand_wildcard(char **token_cmd,
									t_shell *shell);
int								exec_cmd(char **cmd, t_shell *shell, char *pv);
int								check_error_token(char *s);
int								is_only_backslashes_or_backtick(char *s);
void							copy_without_quotes(char *s, char *clean);
int								err_curly2(void);
int								err_curly2(void);
int								count_backslash(char *s, int pos);

int								is_escaped(char *s, int pos);
void							process_backslash(const char *s, char *clean,
									int *i, int *j);

char							*parse_tilde(char *in, int *i,
									char *w, t_shell *shell);
void							handle_backslash_sequence_in_doublequote(
									char *in, int *i, char **w,
									t_shell *shell);

char							*process_escapes(char *input);
int								validate_only_backslash_tick(t_shell *shell,
									char *str);
char							*remove_empty_backticks(char *s);
void							handle_dollar_braces(char *in, int *i,
									char **w, t_shell *shell);
int								handle_closed_backtick(char *s, int *i,
									char *res, int *j);
int								skip_quotes(char c, int *s, int *d);
void							handle_backslash(char *in, int *i, char **w);
int								handle_backslash3(char *s, int i, int state[4]);
void							get_shlvl(t_shell *shell);
void							execute_tokens(t_shell *shell, char **toks);
int								is_control_operator(char *token);
void							build_cmd(t_shell *shell, char **toks,
									int count);
void							execute_single_command(t_shell *shell);
int								handle_normal(char *s,
									int i, int state[4]);
void							capture_parentheses_content(
									char *in, int *i, char **w);
void							handle_doublequote(char *in, int *i,
									char **w, t_shell *shell);
int								contains_star(char *s, int start, int end);
void							handle_single_escape(char c, char **w);
void							execute_command(t_shell *shell,
									char **toks, int *i, int *exec_next);
int								is_subshell(char *toks);
void							execute_subshell(char *toks,
									t_shell *shell);

#endif
