#ifndef COMMAND_H
#define COMMAND_H
#include "Parser.h"
#include "SymbolTable.h"
#include "Interpreter.h"
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <math.h>
using namespace std;


int createServer(int);
void createClient();
double createExp(vector<string>, int, int);
bool isBooleanOp(string);
bool evaluateCondition(double, double, string);
void createCommands();

class Command {
public:
    virtual int execute(int, vector<string>) = 0;
    
};

class OpenServerCommand : public Command {   
public:
    int execute(int , vector<string>);
    
    
};

class ConnectCommand : public Command {
public:
    int execute(int index, vector<string> tokens) {
        
        while(tokens[index] != "#")
            index++;
        
        thread t1(createClient);
        t1.detach();
        return index + 1;
    }
};

class PrintCommand : public Command {
public:
    int execute(int index, vector<string> tokens);
        
};

class SleepCommand : public Command {
public:
    int execute(int index, vector<string> tokens) {
        index += 2;
        int interval = stoi(tokens[index]);
        this_thread::sleep_for(chrono::milliseconds(interval));
        while(tokens[index] != "#")
            index++;
        return index + 1;
    }
};

class IfCommand : public Command {
public:
    int execute(int index, vector<string> tokens);
};

class WhileCommand : public Command {
public:
    int execute(int index, vector<string> tokens);
};

class VarCommand : public Command {
public:
    int execute(int index, vector<string> tokens);
};

class AssignCommand : public Command {
public:
    int execute(int index, vector<string> tokens);
};


#endif