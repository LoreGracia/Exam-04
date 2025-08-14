/*
Assignment name: ft_popen
Expected files:	ft_popen.c
Allowed functions: pipe, fork, dup2, execvp, close, exit

--------------------------------------------------------------------------------------
write the following function:

    int    ft_popen(const char file, char const argv[], char type)

The function must launch the executable file with the arguments argv (using execvp).
If the type is 'r' the function must return a file descriptor connected to the output of the command.
If the type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

example:

int main() {
    int fd = ft_popen("ls", (char const[]){"ls", NULL}, 'r');

    charline;
    while(line = get_next_line(fd))
        ft_putstr(line);
}

Hint: Do not leak file descriptors!
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int ft_popen(const char *file, char *const argv[], char type)
{
  int pid;
  int fd_pipe;

  pipe(fd_pipe);
  pid = frok();
  if (pid == -1)
    return (-1);
  if (!pid)
  {
    if (type == 'w')
      dup2(fd_pipe[0], 0);
    if (type == 'r')
      dup2(fd_pipe[1], 1);
    close(fd_pipe[0]);
    close(fd_pipe[1]);
    execvp(file, argv);
    exit(-1);
  }
  if (type == 'w')
    return (close(fd_pipe[0]), fd_pipe[1]);
  else
    return (close(fd_pipe[1]), fd_pipe[0]);
  return (-1);
}
