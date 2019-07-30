#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
using namespace std;
//模拟实现SetNoBlock函数，将文件描述符设为非阻塞方式
void SetNoBlock(int fd)
{
    //获得文件状态标记
    int f1 = fcntl(fd, F_GETFL);
    if(f1 < 0)
    {
        cerr << "Get Error..." << endl;
    }
    //将f1状态的文件描述符fd设置为非阻塞
    fcntl(fd, F_SETFL, f1 | O_NONBLOCK);
}
int main()
{
    SetNoBlock(0);
    while(1)
    {
        sleep(1);
        char buf[1024] = {0};
        int ret = read(0, buf, sizeof(buf)-1); 
        if(ret < 0)
        {
            //读事件未就绪，把错误读到errno中，EAGAIN的值为11
            if(errno == EAGAIN)
            {
                cout << "data not ready, try again" << endl;
                continue;
            }
            cerr << "read error " << endl;
            continue;
        }
        else if(ret == 0)
        {
            cout << "read file end " << endl;
            break;
        }
        else
        {
            cout << ret << endl;
            cout << buf << endl;
            //错误原因
            cout << errno << endl;
        }
    }
    return 0;
}
