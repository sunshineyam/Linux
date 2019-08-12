#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <error.h>
int main()
{
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        perror("fork error\n");
        return 1;
    }
    else if(pid == 0)
    {
        //child
        printf("child is run,pid is :%d\n",getpid());
        sleep(5);
        exit(210);
    }
    else
    {
        //parents
        int status = 0;
        //阻塞式等待任意一个子进程
        pid_t ret = waitpid(-1, &status, 0);
        printf("wait...\n");
        if(WIFEXITED(status) && ret == pid)
        {
            //等成功，提取出退出码
            printf("wait success, child return code is :%d.\n", WEXITSTATUS(status));
        }
        else
        {
            printf("wait failed...\n");
            return 1;
        }
    }
    return 0;
}
