/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:17:47 by lumartin          #+#    #+#             */
/*   Updated: 2025/02/28 14:41:01:0 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	automata(t_token *tokens)
{
	int		current_state;
	int		prev_token;
	char	*elements[9] = {"command", "file", "|", "newline", "newline",
			"newline", "newline", "err"};

	tokens = tokens->next;
	int automata[6][8] = {
		{1, 5, 5, 3, 3, 3, 3, 2}, // inicial
		{2, 1, 4, 3, 3, 3, 3, 1}, // comando
		{2, 2, 4, 3, 3, 3, 3, 2}, // file
		{2, 2, 5, 5, 5, 5, 5, 2}, // redireccion
		{1, 5, 5, 3, 3, 3, 3, 2}, // pipe
		{5, 5, 5, 5, 5, 5, 5, 5}  // err
	};                            // w  f  |  <  > <<  >> $
	current_state = 0;
	while (current_state != 5 && tokens != NULL)
	{
		prev_token = tokens->type;
		current_state = automata[current_state][tokens->type];
		tokens = tokens->next;
	}
	if (current_state != 1 && current_state != 2)
	{
		printf("syntax error: %s\n", elements[prev_token]);
		return ;
	}
	else
		printf("correct\n");
}

void	make_command(t_token *tokens, char **env)
{
	char	*args;
	char	**full_command;
	int		i;
	pid_t	pid;
	int		status;
	char	*temp1;
	char	*temp2;

	args = ft_strdup("");
	if (!args)
		return ;
	tokens = tokens->next;
	while (tokens != NULL)
	{
		temp1 = args;
		args = ft_strjoin(args, tokens->content);
		free(temp1);
		temp2 = args;
		args = ft_strjoin(args, " ");
		free(temp2);
		tokens = tokens->next;
	}
	full_command = ft_split(args, ' ');
	free(args);
	if (!full_command)
		return ;
	i = 0;
	while (full_command[i] != NULL)
	{
		printf("[%d]  %s\n", i, full_command[i]);
		i++;
	}
	pid = fork();
	if (pid == -1) // error
	{
		printf("Error: fork failed\n");
		i = 0;
		while (full_command[i])
			free(full_command[i++]);
		free(full_command);
		return ;
	}
	else if (pid == 0) // hijo
	{
		if ( ft_strncmp(full_command[0], "echo", 5) == 0
			|| ft_strncmp(full_command[0], "cd", 3) == 0
			|| ft_strncmp(full_command[0], "pwd", 4) == 0
			|| ft_strncmp(full_command[0], "env", 4) == 0
			|| ft_strncmp(full_command[0], "export", 7) == 0
			|| ft_strncmp(full_command[0], "unset", 6) == 0
			|| ft_strncmp(full_command[0], "exit", 5) == 0)
			handle_builtin(full_command, env);
		else
			exe(env, full_command);
		i = 0;
		while (full_command[i])
			free(full_command[i++]);
		free(full_command);
		exit(1);
	}
	else // padre
	{
		waitpid(pid, &status, 0);
		i = 0;
		while (full_command[i])
			free(full_command[i++]);
		free(full_command);
	}
}
