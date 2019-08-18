#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
void Usage(string proc)
{
    cerr << proc << "SERVER_IP SERVER_PORT" <<endl;
    cerr << "Version: 0.0.1" <<endl;
}
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        cerr << "sock error" << endl;
        return 2;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    seever.sin_addr.s_addr = inet_addr(argv[1]);
    //客户端不需要绑定端口号，由OS动态分配，若绑定了端口号，当客户端启动失败，端口号被别的客户端占用了，就会启动不起来了。
    char buf[1024];
    for(;;)
    {
        cout << "Please Enter#";
        cin >> buf;
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0,(struct sockaddr*)&peer, &len);
        if(s > 0)
        {
            buf[s] = 0;
            cout << "server echo#" << buf << endl;
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&peer, len);
        }
    }
    close(sock);
    return 0;
}
