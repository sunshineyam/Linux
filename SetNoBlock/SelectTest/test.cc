//检测标准输入输出
#include <iostream>
#include <sys/select.h>
#include <unistd.h>
#include <string>
using namespace std;
int main()
{
    fd_set rfds;
    //每次设置文件描述符集中的fd的位时都要先清空
    FD_ZERO(&rfds);
    FD_SET(1, &rfds);
    for(;;)
    {
        int ret = select(2, &rfds, NULL, NULL, NULL);
        if(ret < 0)
        {
            cerr << "select error " << endl;
            continue;
        }
        //就绪
        if(FD_ISSET(1, &rfds) > 0)
        {
            char buf[1024] = {0};
            int read_size = read(1,buf,sizeof(buf)-1);
            cout << buf << endl;
        }
        else
        {
            cerr << "data not ready..." << endl;
            continue;
        }
        //每次调用完select都要把原来的文件描述符集清空
        FD_ZERO(&rfds);
        FD_SET(1, &rfds);
    }
    return 0;
}
