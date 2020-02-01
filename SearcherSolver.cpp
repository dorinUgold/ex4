#include "SearcherSolver.h"

string SearcherSolver::solve(Searchable<Point>* p) {
    return searcher->search(p);
}
