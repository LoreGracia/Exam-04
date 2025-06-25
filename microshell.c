/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgracia- <lgracia-@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:47:22 by lgracia-          #+#    #+#             */
/*   Updated: 2025/06/25 14:58:23 by lgracia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define ERROR "error: fatal\n"

typedef struct	s_micro
{
	char **env;
	int	tmp[2];
	int	fd;
	int	num_pipes;
}				t_micro;

void	ft_putstr_fd(int fd, char *a)
{
	int	i;

	i = -1;
	while (a[++i])
		write(fd, &(a[i]), 1);
}

int	exec(t_micro *micro, char **av, int *i)
{
	dup2(micro->fd, 0);
	av[*i] = NULL;
	close(micro->fd);
	execve(av[0], av, micro->env);
	ft_putstr_fd(2, "error: cannot execute ");
	ft_putstr_fd(2, av[0]);
	ft_putstr_fd(2, "\n");
	return (1);
}

void	cd_builtin(t_micro *micro, char **av, int *i)
{
	if (*i != 2)
		ft_putstr_fd(2, "error: cd: bad arguments\n");
	else if (chdir(av[1]) == -1)
	{
		ft_putstr_fd(2, "error: cd: cannot change directory ");
		ft_putstr_fd(2, av[1]);
		write(2, "\n", 1);
	}
}

int	no_pipe(t_micro *micro, char **av, int *i)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd(2, ERROR), 1);
	if (!pid)
	{
		if (exec(micro, av, i) == -1)
			return (-1);
	}
	else
	{
		close(micro->fd);
		while (micro->num_pipes > 0)
		{
			waitpid(-1, NULL, WUNTRACED);
			micro->num_pipes--;
		}
		micro->fd = dup(0);
	}
	return (0);
}

int	is_pipe(t_micro *micro, char **av, int *i)
{
	int	pid;
	
	micro->num_pipes++;
	pipe(micro->tmp);
	if (pid == -1)
		return (ft_putstr_fd(2, ERROR), 1);
	pid = fork();
	if (!pid)
	{
		close(micro->tmp[0]);
		dup2(micro->tmp[1], 1);
		if (exec(micro, av, i))
			return (-1);
	}
	else
	{
		close(micro->tmp[1]);
		close(micro->fd);
		micro->fd = micro->tmp[0];
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_micro	micro;
	int	i;

	if (!av[1])
		return (0);
	i = 0;
	micro.env = env;
	micro.num_pipes = 1;
	micro.fd = dup(0);
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (*av && av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (*av && !strcmp(av[0], "cd"))
			cd_builtin(&micro, av, &i);
		else if (*av && (!av[i] || !strcmp(av[i], ";")))
		{
			if (no_pipe(&micro, av, &i))
				return (-1);
		}
		else if (*av && av[i] && !strcmp(av[i], "|"))
		{
			if (is_pipe(&micro, av, &i))
				return (-1);
		}
	}
	close(micro.fd);
	waitpid(-1, NULL, WUNTRACED);
	return (0);
}
