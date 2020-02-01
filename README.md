Readme - Ex4
------------

Compiling an Running
--------------------

The main.cpp file contains the main function.
The main perfomrs the following steps - 
1. Opens the parallel server
2. Creates the search algorithm.
3. Pass the client handler.



I've used clang++ compiler, you can use this line - 

clang++ main.cpp Server.h MySerialServer.cpp MySerialServer.h ClientHandler.h MyTestClientHandler.h MyTestClientHandler.cpp Solver.h CacheManager.h FileCacheManager.h FileCacheManager.cpp StringReverser.cpp StringReverser.h State.h Searchable.h Searcher.h BestFirstSearch.h Matrix.cpp Matrix.h Point.cpp Point.h MyClientHandler.cpp MyClientHandler.h SplitClass.h SearcherSolver.cpp SearcherSolver.h BFS.h DFS.h A_Star.h MyParallelServer.cpp MyParallelServer.h -std=c++11

All output is done on the terminal.
