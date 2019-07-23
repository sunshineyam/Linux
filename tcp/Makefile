.PHONY:all
all:TcpClient TcpServer

TcpClient:TcpClient.cc
	g++ -o $@ $^
TcpServer:TcpServer.cc
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f TcpClient TcpServer
