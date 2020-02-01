#ifndef MY_SERIAL_SERVER_H
#define MY_SERIAL_SERVER_H

#include "ClientHandler.h"
#include "Server.h"
#include <cstdio>
#include <cstdlib>

#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>

#include <cstring>

#include <sys/socket.h>


class MySerialServer : public server_side::Server {
public:
    void open(int port, ClientHandler* c) override;
    void stop() override;
    static void serialService(int sockfd,ClientHandler* c);

};


#endif 