#include "comm.h"
int main()
{
    //客户端不需要再创建共享内存
    int shmid = getShm(4096);
    sleep(1);
    char* addr = shmat(shmid, NULL, 0);
    sleep(2);
    int i = 0;
    while(i < 26)
    {
        //客户端往共享内存中写
        addr[i] = 'A' + i;
        i++;
        //保证字符串每次以\0结尾
        addr[i] = 0;
        sleep(1);
    }
    shmdt(addr);
    sleep(2);
    return 0;
}
