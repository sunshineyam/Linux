#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[])
{
    int fds[2];
    if(pipe(fds) < 0)
    {
        perror("pipe error\n");
        exit(1);
    }
    pid_t pid = fork();
    //子进程
    if(pid == 0)
    {
        close(fds[0]);
        write(fds[1], "hello", 5);
        close(fds[1]);
        exit(2);
    }
    close(fds[1]);
    char buf[10] = {0};
    read(fds[0], buf, 10);
    printf("buf = %s\n", buf);
    return 0;
}
