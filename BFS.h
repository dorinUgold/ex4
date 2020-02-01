#ifndef BFS_H
#define BFS_H

#include "Searcher.h"
#include <algorithm>
#include <list>
#include <queue>

template<class T>
class BFS : public Searcher<T> {
public:
    string search(Searchable<T> *searchable) {
        vector<State<T> *> visited;
        queue<State<T>*> queue;
        vector<State<T> *> neighbors;
        State<T> *current;
        
        queue.push(searchable->getInitialState());
        visited.push_back(searchable->getInitialState());

        while (!queue.empty()) {
            
            current = queue.front();
            
            if(current->Equals(searchable->getGoalState())){
                cout<<"num of nodes: " << this->numOfNodesEvaluated<<endl;
                this->numOfNodesEvaluated++;
                return searchable->getPath();
            }
            neighbors = searchable->getNeighbors(current);
            queue.pop();
            this->numOfNodesEvaluated++;

            for (State<T> *neighbor : neighbors) {
                //if a neighbors has not been visited then mark as visited and add to queue list
                if (!inVisited(visited,neighbor)) {
                    neighbor->setComeFrom(current);
                    neighbor->addCost(current->getTrailCost());
                    visited.push_back(neighbor);
                    queue.push(neighbor);
                }
            }
        }

        return "-1";
    }

    bool inVisited(vector<State<T> *> visited,State<T> *current ){
        for (auto state:visited) {
            if (current->Equals(state)) {
                return true;
            }
        }
        return false;
    }

    string getPath(vector<State<T> *> path){
        string p;
        reverse(p.begin(), p.end());
        typename vector<State<T>*>:: iterator it;
        long i = 1;
        for (it = path.begin() ; it < path.end(); it++) {
            if (i == path.size()) {
                break;
            }
            State<T>* next = path[i];
            p += (*it)->getState().move(next->getState());
            i++;
        }
        reverse(p.begin(), p.end());
        return p;
    }
};

#endif
