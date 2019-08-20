#include "comm.h"
int main()
{
    //操作系统以页为单位分配共享内存，1页=4k=4096kb,但是当改成4097时并不会分配两个页，而是类似于写时拷贝
    int shmid = createShm(4096);
    char* addr = shmat(shmid, NULL, 0);
    sleep(2);
    int i = 0;
    while(i++ < 26)
    {
        //服务器负责从共享内存中读
        printf("client# %s\n", addr);
        sleep(1);
    }
    shmdt(addr);
    sleep(2);
    destroyShm(shmid);
    return 0;
}
