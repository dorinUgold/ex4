#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <unistd.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include "MyTestClientHandler.h"

MyTestClientHandler::MyTestClientHandler(Solver<string,string>* solver,FileCacheManager* fileCacheManager) {
    this->solver = solver;
    this->cacheManager = fileCacheManager;
}

void MyTestClientHandler::handleClient(int socketId) {
    char prob[256];
    string solution;
    ssize_t n;
    char* chr;
    if (socketId < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    while (true) {
        /* If connection is established then start communicating */
        bzero(prob, 256);
        n = read(socketId, prob, 255);

        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        if (strcmp(prob, "end") == 0) {
            return;
        }

        printf("Here is the message: %s\n", prob);

        //get solution from disk
        if (this->cacheManager->hasSolution(prob)) {
            solution = this->cacheManager->getSolution(prob);
        } else {
            solution = solver->solve(prob);
            cacheManager->updateData(prob,solution);
            cacheManager->saveToDisk(prob, solution);
        }
        /* Write a response to the client */
        chr = const_cast<char *>(solution.c_str());
        n = write(socketId, chr, strlen(chr));
        cout<<solution<<endl;

        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
    }
}