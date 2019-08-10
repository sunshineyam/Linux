#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int gal = 0;
int main()
{
    pid_t id  = fork();
    if(id == 0)
    {
        //child
        gal = 1;
        printf("child[%d] : %d : %p\n",getpid(), gal, &gal);
    }
    else if(id > 0)
    {
        //parents
        sleep(3);
        printf("parents[%d] : %d : %p\n",getpid(), gal, &gal);
    }
    else
    {
        perror("fork");
        exit(0);
    }
    sleep(1);
    exit(1);
}
