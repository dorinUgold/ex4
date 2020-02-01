#ifndef SERVER_H
#define SERVER_H

#include "ClientHandler.h"

namespace server_side {
    class Server {

    public:
        virtual void open(int port, ClientHandler *c) = 0;

        virtual void stop() = 0;

        virtual ~ Server() = default;
    };
}

#endif 
