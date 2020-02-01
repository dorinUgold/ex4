
#include <iostream>
#include "Solver.h"
#include "BestFirstSearch.h"
#include "SearcherSolver.h"
#include "MyClientHandler.h"
#include "A_Star.h"
#include "BFS.h"
#include "DFS.h"
#include "MyParallelServer.h"
#include "FileCacheManager.h"

namespace boot {
    class Main {
    public:
        int main(int argc, char *argv[]) {
            return 0;
        }
    };
}

int main(int argc, char *argv[]) {
    boot::Main m;
    m.main(argc, argv);
    int num = 1;
    server_side::Server *server = new MyParallelServer();
    Solver<Searchable<Point> *, string> *solver = new SearcherSolver(new A_Star<Point>());
   // Solver<Searchable<Point> *, string> *solver = new SearcherSolver(new BFS<Point>());
   // Solver<Searchable<Point> *, string> *solver = new SearcherSolver(new DFS<Point>());
   // Solver<Searchable<Point> *, string> *solver = new SearcherSolver(new BestFirstSearch<Point>());
    CacheManager *cacheManager = new FileCacheManager();
    ClientHandler *clientHandler = new MyClientHandler(solver, cacheManager);
    num++;
    server->open(stoi(argv[1]), clientHandler);
    delete server;
    delete solver;
    delete cacheManager;
    delete clientHandler;

    return 0;
}