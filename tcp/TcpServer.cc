#include "TcpServer.hpp"
using namespace std;
void Usage(string proc)
{
    cout << "Usage: " << proc << "ip port" << endl;
}
//.seerver ip port
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    int port = atoi(argv[2]);
    string ip = argv[1];
    TcpServer *tsp = new TcpServer(ip, port);
    tsp->InitServer();
    tsp->Start();
    return 0;
}

