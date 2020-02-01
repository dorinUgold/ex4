#include "Matrix.h"

Matrix::Matrix(std::vector<State<Point>*> structure, State<Point>* initialState,State<Point>* goal) {
    this->structure = structure;
    this->initialState = initialState;
    this->goal = goal;
}

vector<State<Point>*> Matrix::getNeighbors(State<Point>* s){
    vector<State<Point>*> neighbors;
    for (State<Point>* state : structure) {
        if (state->getState().isLeftNeighbor(s->getState()) || state->getState().isRightNeighbor(s->getState())
            || state->getState().isUpNeighbor(s->getState()) || state->getState().isDownNeighbor(s->getState())) {
            if (state->getCost() != -1) {
                neighbors.push_back(state);
            }
        }
    }
    return neighbors;
}

Matrix::~Matrix(){
    for (auto state: structure){
        delete state;
    }
}
