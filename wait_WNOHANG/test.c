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
        exit(1);
    }
    else
    {
        //parents
        int status = 0;
        pid_t ret = 0;
        do
        {
            //非阻塞式等地，没有已退出的子进程，返回值是0
            ret = waitpid(-1, &status, WNOHANG);
            if(ret == 0)
            {
                printf("child is running...\n");
            }
            sleep(1);
        }while(ret == 0);
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
