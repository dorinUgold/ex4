#ifndef DFS_H
#define DFS_H

#include "Searcher.h"
#include <stack>
using namespace std;

template <class T>
class DFS : public Searcher<T> {
public:
    string search(Searchable<T> *searchable) {
        vector<State<T> *> visited;
        stack<State<T> *> stack;
        vector<State<T> *> neighbors;
        State<T> *current;

        stack.push(searchable->getInitialState());

        while (!stack.empty()) {
            current = stack.top();
            stack.pop();
            this->numOfNodesEvaluated++;

            if (!inVisited(visited, current)) {
                visited.push_back(current);
            }

            if(current->Equals(searchable->getGoalState())) {
                cout<<"num of nodes: " << this->numOfNodesEvaluated<<endl;
                this->numOfNodesEvaluated++;
                return searchable->getPath();
            }

            neighbors = searchable->getNeighbors(current);
            for (State<T> *neighbor : neighbors) {
                //if a neighbors has not been visited then mark as visited and add to queue list
                if (!inVisited(visited,neighbor)) {
                    neighbor->setComeFrom(current);
                    neighbor->addCost(current->getTrailCost());
                    stack.push(neighbor);
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
};

#endif 
