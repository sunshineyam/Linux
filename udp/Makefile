.PHONY:all
all:udpClient udpServer
CC=g++

udpClient:udpClient.cc
	$(CC) -o $@ $^ 
udpServer:udpServer.cc
	$(CC) -o $@ $^

.PHONY:clean
clean:
	rm -f udpClient udpServer
