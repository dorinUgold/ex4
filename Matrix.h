#ifndef MATRIX_H
#define MATRIX_H

#include "Searchable.h"
#include "Point.h"

class Matrix : public Searchable<Point>{

public:
    Matrix(std::vector<State<Point>*> structure, State<Point>* initialState,State<Point>* goal);
    vector<State<Point>*> getNeighbors(State<Point>* s) override;
    ~Matrix() override;
};


#endif 
