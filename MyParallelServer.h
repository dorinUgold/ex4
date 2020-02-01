#ifndef MY_PARALLEL_SERVER_H
#define MY_PARALLEL_SERVER_H

#include <vector>
#include "Server.h"
#include <pthread.h>
#include <stack>

class MyParallelServer : public server_side::Server {
private:
    struct params {
        ClientHandler* clientHandler;
        int sockfd;
    };
    struct params *info = new params();
    int serverId;
    stack<pthread_t> threads;


public:
void open(int port, ClientHandler *c) override;
void stop() override;
void start(int server_sock, ClientHandler *ch);
static void* parallelService (void* params);
~MyParallelServer() override {delete info;}
};

#endif 
