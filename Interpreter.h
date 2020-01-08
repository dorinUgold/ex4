#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <iostream>

using namespace std;

class Interpreter {
private:
    queue<string> tokens;
    unordered_map<string, double> vars;
public:
    double interpret(string);
    void setVariables(string);
    void createTokens(string);
};