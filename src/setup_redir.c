/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:25:22 by adrianafern       #+#    #+#             */
/*   Updated: 2025/03/26 18:12:52 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int handle_redir_right(t_token *tokens, int (*fds)[2])
{
    int	new_fd;

    new_fd = open(tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (new_fd == -1)
        return(perror("minishell: open"), ERROR);
    if ((*fds)[1] != STDOUT_FILENO)
        close((*fds)[1]);
    return (new_fd);
}

int handle_redir_append(t_token *tokens, int (*fds)[2])
{
    int new_fd;

    new_fd = open(tokens->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (new_fd == -1)
        return(perror("minishell: open"), ERROR);
    if ((*fds)[1] != STDOUT_FILENO)
        close((*fds)[1]);
    return (new_fd);
}

int handle_redir_left(t_token *tokens, int (*fds)[2])
{
    int new_fd;

    new_fd = open(tokens->next->content, O_RDONLY);
    if (new_fd == -1)
        return(perror("minishell: open"), ERROR);
    if ((*fds)[0] != STDIN_FILENO)
        close((*fds)[0]);
    return (new_fd);
}

int handle_redir_heredoc(t_token *tokens)
{
    int status;
    pid_t	pid;
	int connect[2];

    ign_signal();
    if (pipe(connect) == -1)
        return(perror("pipe"), ERROR);
    pid = fork();
    if (pid == -1)
        return(perror("fork"), ERROR);
    else if (pid == 0)
    {
        signals('h');
        close(connect[0]);
        handle_heredoc(tokens->next->content, connect[1], tokens);
        close(connect[1]);
        exit(0);
    }
    close(connect[1]);
    waitpid(pid, &status, 0);
    signals('f');
    return(connect[0]);
}

void	setup_redirections(t_token *tokens, int (*fds)[2], int count)
{
	tokens = tokens->next;
	while (count > 0) //&& temp_tokens->type (esta condicion daba problemas)
	{
		if (tokens->type == T_PIPE)
			count--;
		tokens = tokens->next;
	}
	while (tokens)
	{
		if (tokens->type == T_REDIR_RIGHT && tokens->next)
			(*fds)[1] = handle_redir_right(tokens, fds);
		else if (tokens->type == T_APPEND && tokens->next)
			(*fds)[1] = handle_redir_append(tokens, fds);
		else if (tokens->type == T_REDIR_LEFT && tokens->next)
			(*fds)[0] = handle_redir_left(tokens, fds);
		else if (tokens->type == T_HERE_DOC && tokens->next)
            (*fds)[0] = handle_redir_heredoc(tokens);
		else if (tokens && tokens->type == T_PIPE)
        	break ;
		tokens = tokens->next;
	}
}
