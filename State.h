#ifndef STATE_H
#define STATE_H

#include <vector>
#include <cfloat>

using namespace std;

template<class T>
class State {
private:
    T state;
    double cost;
    double trailCost;
    double Fheuristics;
    State<T> *comeFrom;


public:
    State<T>(T state, double cost) : state(state), cost(cost), trailCost(cost), comeFrom(nullptr),Fheuristics(FLT_MAX) {}

    double getCost() const { return cost; }

    bool Equals(State<T> *s) { return this->state == s->state; }

    void setComeFrom(State<T> *comeFrom) { this->comeFrom = comeFrom; }

    void addCost(double c) { trailCost += c; }

    double getTrailCost() { return trailCost; }

    void setTrailCost(double cost) { this->trailCost = cost; }

    T getState() { return state; }

    void setCost(double cost) {
        this->cost = cost;
        this->trailCost = cost;
    }

    State<T>* getComaFrom() { return comeFrom; }

    void setFheuristics(double f){
        this->Fheuristics = f;
    }
    double getFheuristics(){
        return Fheuristics;
    }
};


#endif 