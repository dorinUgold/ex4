#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cctype>

using namespace std;

class Lexer {
private:
    string fPath;
    vector<string> tokens;
public:
    Lexer(string);
    vector<string> tokenize();
    void splitLine(string);
    
};

#endif