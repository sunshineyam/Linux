#ifndef _TCP_SERVERE_HPP_
#define _TCP_SERVER_HPP_

#include <iostream>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
using namespace std;
class TcpServer{
private:
    int listen_sock;
    string _ip;
    int _port;
public:
    TcpServer(string ip, int port)
        :_ip(ip)
         ,_port(port)
         ,listen_sock(-1)
    {
    }
    void InitServer()
    {
        listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(listen_sock < 0)
        {
            cerr << "sock error" << endl;
            exit(2);
        }
        //ipv4编程使用的数据结构
        struct sockaddr_in local;
        //结构体清零
        bzero(&local, sizeof(local));
        //地址类型
        local.sin_family = AF_INET;
        //网络服务，端口号要转成网络字节序
        local.sin_port = htons(_port);
        //ip地址
        local.sin_addr.s_addr = inet_addr(_ip.c_str());
        if(bind(listen_sock,(struct sockaddr*)&local, sizeof(local)) < 0)
        {
            cerr << "bind error" << endl;
            exit(3);
        }
        if(listen(listen_sock, 5) < 0)
        {
            cout << "listen error" << endl;
            exit(4);
        }
    }
    //读写服务
    void Service(int sock)
    {
        while(1)
        {
            char buf[1024];
            ssize_t s = read(sock, buf, sizeof(buf)-1);
            if(s > 0)
            {
                buf[s] = 0;
                cout << "client# " << buf << endl;
                write(sock, buf, strlen(buf));
            }
            //客户端关闭，服务器读到0值
            else if(s == 0)
            {
                cout << "client quit" << endl;
                break;
            }
            else
            {
                cerr << "read error" << endl;
            }
        }    
        close(sock);
    }
    void Start()
    {
        struct sockaddr_in peer;
        socklen_t len;
        for(;;)
        {
            len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if(sock < 0)
            {
                cerr << "accept error" << endl;
                continue;
            }
            cout << "Get a new client" << endl;
            Service(sock);
        }
    }
    ~TcpServer()
    {
        if(listen_sock > 0)
        {
            close(listen_sock);
        }
    }
};







#endif
