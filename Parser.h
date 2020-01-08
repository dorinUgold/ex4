#ifndef PARSER_H
#define PARSER_H
#include <map>
#include "Lexer.h"
#include "Command.h"

class Command;

class Parser {
private:
    vector<string> tokens;
    map<string, Command*> commands;
public:
    Parser(vector<string>);
    void parse();

};

#endif