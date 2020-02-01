
#ifndef A_STAR_H
#define A_STAR_H

#include <cfloat>
#include "Searcher.h"
#include <set>
#include <stack>
#include <cstring>
#include <queue>
#include "Point.h"
using namespace std;

template<class T>

class A_Star : public Searcher<T> {
    class Comp {
    public:
        bool operator()(State<T>* left, State<T>* right) {
            return (left->getFheuristics()) > (right->getFheuristics());
        }
    };
public:
    string search(Searchable<T>* searchable) override {
        this->searchable = searchable;
        State<T> *current;
        vector<State<T> *> neighbors;
        priority_queue<State<T>*, vector<State<T>*>, Comp> open;
        //searchable->getInitialState()->setFheuristics(0);
        open.push(searchable->getInitialState());
        vector<State<T> *> closed;

        while (!open.empty()) {
            current = open.top();
            open.pop();
            this->numOfNodesEvaluated++;
            closed.push_back(current);
            if (current->Equals(searchable->getGoalState())) {
                this->numOfNodesEvaluated++;
                return searchable->getPath();
            }
            neighbors = searchable->getNeighbors(current);
            for (State<T> *neighbor : neighbors) {
                //first time encountering state
                if (!inOpen(open, neighbor) && !inClosed(closed, neighbor)) {
                    neighbor->setComeFrom(current);
                    neighbor->addCost(current->getTrailCost());
                    neighbor->setFheuristics(calculateF(neighbor,searchable->getGoalState()));
                    open.push(neighbor);
                    continue;
                    //neighbor is either in open or closed and - can improve path
                } else if (inClosed(closed, neighbor)){
                    continue;
                }
                else if (current->getTrailCost() + neighbor->getCost() < neighbor->getTrailCost()) {
                    neighbor->setTrailCost(current->getTrailCost() + neighbor->getCost());
                    neighbor->setFheuristics(calculateF(neighbor, searchable->getGoalState()));
                    neighbor->setComeFrom(current);
                    open = updatePriorityQ(open);
                }
            }
        }
        searchable->getGoalState()->setTrailCost(-1);
        return "-1";
    }

    bool inOpen(priority_queue<State<T> *, vector<State<T> *>, Comp> p, State<T> *current) {
        while (!p.empty()) {
            //found state in priority/-queue
            if (current->Equals(p.top())) { return true; }
            p.pop();
        }
        return false;
    }

    bool inClosed(vector<State<T>*> closed, State<T>* current) {
        for (auto state:closed) {
            if (current->Equals(state)) {
                return true;
            }
        }
        return false;
    }

    priority_queue<State<T>*, vector<State<T> *>, Comp> updatePriorityQ(priority_queue<State<T> *,
            vector<State<T> *>, Comp> p ){
        priority_queue<State<T> *, vector<State<T> *>, Comp> newQ;
        while (!p.empty()) {
            newQ.push(p.top());
            p.pop();
        }
        return newQ;
    }


    double calculateF(State<T>* state,State<T>* goal) {
        //manhattan distance
        double h = abs(state->getState().getX() - goal->getState().getX()) +
                abs(state->getState().getY() - goal->getState().getY());
        double g = state->getTrailCost();
        return g + h;
    }

};

#endif 
