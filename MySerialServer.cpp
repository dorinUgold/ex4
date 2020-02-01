#include <thread>
#include "MySerialServer.h"

void MySerialServer::open(int port, ClientHandler* c) {
    int sockfd, portno;

    struct sockaddr_in serv_addr;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = port;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    
    /* Now bind the host address using bind() call.*/
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    thread t(serialService, sockfd,c);
    t.detach();
}

void MySerialServer::serialService(int sockfd, ClientHandler* c) {
    int newsockfd, clilen, xlen;
    struct sockaddr_in cli_addr;
    
    xlen = 0;
    while (true) {
        listen(sockfd, SOMAXCONN);
        clilen = sizeof(cli_addr);
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        c->handleClient(newsockfd);
    }
    xlen++;
}

void MySerialServer::stop() {

}