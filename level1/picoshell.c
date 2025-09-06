#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char ***cmds)
{
	int fd = -1;
	int i = 0;
	int tmp[2];
	pid_t pid;

	while (cmds[i])
	{
		pipe(pipe_fd);
		pid = fork();
		if (pid == -1)
			return 1;
		if (!pid)
		{
		close(tmp[0]);
		if (fd != -1)
			dup2(fd, 0);
		close(fd);
		if (cmds[i + 1])
			dup2(tmp[1], 1);
		close(tmp[1]);
		execvp(cmds[i][0], cmds[i]);
		exit(0);//los test lo tiran si devuelves 1
		}
		close(tmp[1]);
		if (fd != -1)
			close(fd);
		if (!cmds[i + 1])
			close(tmp[0]);
		else
			fd = tmp[0];
		i++;
	}
	while (i--)
		wait(NULL);//lost test lo tiran si devuelves 1
	return 0;
}

/*
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int i, cmd_size = 1;
	for (i = 1; i < argc; i++)
	{
		if (strcmp("|", argv[i]) == 0)
			cmd_size++;
	}
	char ***cmds = calloc(cmd_size + 1, sizeof(char **));
	if (!cmds)
	{
		perror("calloc");
		return 1;
	}
	cmds[0] = &argv[1];
	int cmds_i = 1;
	for (i = 1; i < argc; i++)
	{
		if (strcmp("|", argv[i]) == 0)
		{
			argv[i] = NULL;
			cmds[cmds_i++] = &argv[i + 1];
		}
	}
	int ret = picoshell(cmds);
	if (ret)
	{
		perror("picoshell");
	}
	free(cmds);
	return ret;
}
*/
