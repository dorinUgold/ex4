#ifndef SEARCHER_H
#define SEARCHER_H

#include <string>
#include "Searchable.h"
template <class T>
class Searcher {
protected:
    Searchable<T>* searchable;
    int numOfNodesEvaluated = 0;

public:
    virtual string search (Searchable<T>* searchable) = 0;
    double getTrailCost() {
        return searchable->getGoalState()->getTrailCost();
    }
    virtual int getNumOfNodesEvaluated() { return numOfNodesEvaluated;}
    virtual ~Searcher(){ delete searchable;}

};
#endif 
