#ifndef _TCP_CLIENT_HPP_
#define _TCP_CLIENT_HPP_
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
using namespace std;
class TcpClient{
private:
    int _sock;
    string _ip;
    int _port;
public:
    TcpClient(string ip, int port)
        :_ip(ip)
         ,_port(port)
         ,_sock(-1)
    {
    }
    void InitClient()
    {
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if(_sock < 0)
        {
            cerr << "socket error" << endl;
            exit(2);
        }
    }
    void Start()
    {
        struct sockaddr_in peer;
        bzero(&peer, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_port);
        peer.sin_addr.s_addr = inet_addr(_ip.c_str());
        if(connect(_sock, (struct sockaddr*)&peer, sizeof(peer)) < 0)
        {
            cerr << "connect error" << endl;
            exit(3);
        }
        char buf[1024];
        string in;
        while(1)
        {
            cout << "Please Enter# ";
            std::cin >> in;
            write(_sock, in.c_str(), in.size());
            ssize_t s = read(_sock, buf, sizeof(buf)-1);
            if(s < 0)
            {
                buf[s] = 0;
                cout << "Server Echo# " << endl;
            }
        }
    }
    ~TcpClient()
    {
        if(_sock > 0)
        {
            close(_sock);
        }
    }
};



#endif
