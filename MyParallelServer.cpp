#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <strings.h>
#include <cstdio>
#include <thread>
#include <iostream>
#include <unistd.h>

#include "MyParallelServer.h"
#include <stack>

struct thread_data {
    int sock;
    ClientHandler *ch;
};

void MyParallelServer::open(int port, ClientHandler *clientHandler) {
    info->sockfd = port;
    info->clientHandler = clientHandler;
    int server_fd;
    struct sockaddr_in address{};
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int n = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int));
    if (::bind(server_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        perror("socket bind");
        exit(1);
    }

    if (listen(server_fd, 5) == -1) {
        perror("socket listen");
        exit(1);
    }

    start(server_fd, clientHandler);
}

void *start_thread_client(void *params) {
    auto data = (thread_data *) params;
    data->ch->handleClient(data->sock);
    delete data;
}

void MyParallelServer::start(int server_sock, ClientHandler *ch) {
    stack<pthread_t> threads_stack;
    sockaddr_in address{};
    int addrlen = sizeof(address);
    timeval timeout;
    int new_socket;
    while (true) {
        new_socket = accept(server_sock, (struct sockaddr *) &address, (socklen_t *) &addrlen);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        int x = 10;
        setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));
        if (new_socket < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                cout << "timeout" << endl;
                stop();
                break;
            }
            perror("accept");
            exit(EXIT_FAILURE);
        }
        setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));
        auto data = new thread_data;
        data->ch = ch;
        data->sock = new_socket;
        pthread_t trid;
        if (pthread_create(&trid, nullptr, start_thread_client, data) < 0) {
            perror("error on creating thread");
            exit(1);
        }
        threads_stack.push(trid);
    }
}
/*
 * making all the clients wait till all are done
 */
void MyParallelServer::stop() {
    while (!threads.empty()) {
        pthread_join(threads.top(), nullptr);
        threads.pop();
    }
}