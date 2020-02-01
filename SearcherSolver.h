#ifndef SEARCHER_SOLVER_H
#define SEARCHER_SOLVER_H

#include "Solver.h"
#include "Searcher.h"
#include "Point.h"


class SearcherSolver : public Solver<Searchable<Point>*,string>{
private:
    Searcher<Point>* searcher;
public:
    SearcherSolver(Searcher<Point>* searcher): searcher(searcher) {}
    string solve(Searchable<Point>* p) override;
    ~SearcherSolver()override{ delete searcher;}
};


#endif 
