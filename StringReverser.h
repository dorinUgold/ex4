#ifndef STRING_REVERSER_H
#define STRING_REVERSER_H

#include <string>
#include "Solver.h"

using namespace std;

class StringReverser : public Solver<string,string>{
public:
    string solve(string problem) override;

};


#endif 
