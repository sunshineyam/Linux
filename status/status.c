#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/wait.h>
int main(void)
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid == 0)
    {
        sleep(20);
        exit(10);
    }
    else
    {
        int st;
        int ret = wait(&st);
        if(ret > 0 && (st & 0X7F) == 0)
        {
            printf("child exit code: %d\n", (st>>8)&0XFF);
        }
        else if(ret > 0)
        {
            printf("sig code: %d\n", st&0X7F);
        }
    }
}
