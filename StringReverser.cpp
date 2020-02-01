#include <algorithm>
#include "StringReverser.h"

string StringReverser::solve(string problem) {
    reverse(problem.begin(), problem.end());
    return problem;
}
