#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//从键盘读取数据，写入管道，读取管道，写到屏幕
int main(void)
{
    int fds[2];
    char buf[100];
    int len;
    if(pipe(fds) < 0)
    {
        perror("make pipe error\n");
        exit(1);
    }

    //read from stdin
    while(fgets(buf, 100, stdin))
    {
        len = strlen(buf);
        //write into pipe
        if(write(fds[1], buf, len) != len)
        {
            perror("write to pipe error\n");
            break;
        }
        memset(buf, 0X00, sizeof(buf));
        //read from pipe
        if(len = read(fds[0], buf, 100) == -1)
        {
            perror("read from pipe error\n");
            break;
        }
        //write to stdout
        if(write(1, buf, len) != len)
        {
            perror("write to stdout error\n");
            break;
        }
    }
    return 0;
}
