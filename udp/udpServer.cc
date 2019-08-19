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
    cerr << proc << "IP PORT" <<endl;
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
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));
    local.sin_addr.s_addr = inet_addr(argv[1]);
    
    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) <0 )
    {
        cerr << "bind error" << endl;
        return 3;
    }

    char buf[1024];
    for(;;)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0,(struct sockaddr*)&peer, &len);
        if(s > 0)
        {
            buf[s] = 0;
            cout << "client#" << buf << endl;
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&peer, len);
        }
    }
    close(sock);
    return 0;
}
