#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main()
{
    //close(0);
    //close(2);
    //int fd = open("myfile", O_RDONLY);
    int fd = open("./Log", O_CREAT | O_RDWR);
    if(fd < 0)
    {
        perror("open error");
        return 1;
    }
    close(1);
    printf("%d\n", fd);
    dup2(fd, 1);
    for(;;)
    {
        char buf[1024] = {0};
        ssize_t read_size = read(0, buf, sizeof(buf)-1);
        if(read_size < 0)
        {
            perror("read");
            break;
        }
        printf("%s", buf);
        fflush(stdout);
    }
    //close(fd);
    return 0;
}
