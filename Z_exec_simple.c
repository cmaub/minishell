/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exec_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anvander < anvander@student.42.fr >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:02:42 by cmaubert          #+#    #+#             */
/*   Updated: 2024/11/04 12:10:04 by anvander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*
- create a **char with the commands and its option / arguments
- search for the path of the command
- execute the cmd with its option and arguments
*/

int	execute(PARSER *current, t_pipex *p)
{
	char	*path;

	if (ft_strchr(current->command[0], '/') || no_envp(p->envp))
	{
		if (access(current->command[0], F_OK | R_OK) == -1)
		{
			free(current);
			perror("access");
			return (-1);
		}
		execve(current->command[0], current->command, NULL);
	}
	else if (!ft_strchr(current->command[0], '/') && !no_envp(p->envp))
	{
		path = get_path_and_check(&current->command[0], p->envp);
		dprintf(2, "path = %s, split_cmd = %s, %s, %s, %s\n", path, current->command[0], current->command[1], current->command[2], current->command[3]);
		if (execve(path, current->command, p->envp) == -1)
		{
			// split_cmd[1] = NULL;
			dprintf(2, "(%s, %d)\n", __FILE__, __LINE__);
			free(current);
			free(path);
		}
	}
	perror("exec");
	return (-1);
}

// int	handle_input_redirection(t_pipex *p, PARSER *current, char *heredoc)
// {
// 	int	fd_in;
// 	fd_in = -1;
// 	(void)heredoc;
	
// 	if (heredoc)
// 		fd_in = open(heredoc, O_RDONLY | 0644);
// 	else
// 		fd_in = open(current->infile, O_RDONLY | 0644);
// 	if (fd_in == -1)
// 	{
// 		safe_close(p->pipefd[0]);
// 		safe_close(p->pipefd[1]);
// 		ft_error("open");
// 	}
// 	if (dup2(fd_in, STDIN_FILENO) == -1)
// 	{
// 		safe_close(p->pipefd[0]);
// 		safe_close(p->pipefd[1]);
// 		safe_close(fd_in);
// 		ft_error("dup2");
// 	}
// 	safe_close(fd_in);
// 	return (fd_in);
// }

// /*
// This function opens fd_out or stops if no output redirection
// then dup2 and close fd_out
// */
// void	handle_output_redirection(PARSER *current, PARSER *nodes, t_pipex *p, int fd_in)
// {
// 	int	fd_out;
// 	fd_out = -1;
// 	(void)nodes;

// 	if (current->outfile)
// 	{
// 		if (current->redir_type_out == REDIRECT_OUT)
// 			fd_out = open(current->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		else if (current->redir_type_out == APPEND_OUT)
// 			fd_out = open(current->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		else
// 			return ;
// 		if (fd_out == -1) // fermer les fd ouverts precedemment
// 		{
// 			safe_close(p->pipefd[0]);
// 			safe_close(p->pipefd[1]);
// 			ft_error("open");
// 		}
// 		if (dup2(fd_out, STDOUT_FILENO) == -1) 
// 		{
// 			safe_close(p->pipefd[0]);
// 			safe_close(p->pipefd[1]);
// 			safe_close(fd_out);	
// 			ft_error("dup2");
// 		}
// 	}
// 	else
// 	{
// 		if (dup2(p->pipefd[1], STDOUT_FILENO) == -1)
// 			ft_close_error(&fd_in, p, "dup2");
// 	}
// 	// safe_close(fd_in);
// 	// safe_close(p->pipefd[1]);		
// 	safe_close(fd_out);
// }

// int	simple_cmd(t_pipex *p, char *heredoc, PARSER *current, PARSER *nodes)
// {
// 	pid_t		pid;
// 	int		status;
// 	int		fd_in;
	
// 	pid = fork();
// 	if (pid == - 1)
// 		ft_error("fork");
// 	else if (pid == 0)
// 	{
// 		if (current->redir_type_in == REDIRECT_IN || current->redir_type_in == HEREDOC)
// 			fd_in = handle_input_redirection(p, current, heredoc);
// 		if (current->outfile)
// 			handle_output_redirection(current, nodes, p, fd_in);
// 		execute(current, p);		
// 	}
// 	waitpid(pid, &status, 0);
// 	// return (ft_wait(pid));
// 	return (0);
// }
