
#ifndef MY_CLIENT_HANDLER_H
#define MY_CLIENT_HANDLER_H

#include "ClientHandler.h"
#include "Solver.h"
#include "CacheManager.h"
#include "Searchable.h"
#include "Point.h"

class MyClientHandler : public ClientHandler{
private:
    Solver<Searchable<Point>*,string>* solver;
    CacheManager* cacheManager;
public:
    MyClientHandler(Solver<Searchable<Point>*,string>* solver,CacheManager* cacheManager);
    void handleClient(int socketId) override;
    Searchable<Point>* makeMatrix(vector<string> tempProb);
};


#endif 
