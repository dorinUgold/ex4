#ifndef SOLVER_H
#define SOLVER_H

template <class P, class S>
class Solver {
public:
    virtual S solve(P prob) = 0;
    virtual ~Solver() = default;
};

#endif 
