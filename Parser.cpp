#include "Parser.h"

Parser::Parser(vector<string> v) {
    // create map of commands
    tokens = v;
    commands.insert(pair<string, Command*>
                ("openDataServer", new OpenServerCommand()));
    commands.insert(pair<string, Command*>
                ("connectControlClient", new ConnectCommand()));
    commands.insert(pair<string, Command*>
                ("var", new VarCommand()));
    commands.insert(pair<string, Command*>
                ("Print", new PrintCommand()));
    commands.insert(pair<string, Command*>
                ("Sleep", new SleepCommand()));
    commands.insert(pair<string, Command*>
                ("if", new IfCommand()));
    commands.insert(pair<string, Command*>
                ("while", new WhileCommand()));
}

void Parser::parse() {
    int i = 0;
    // for each token parse its command and execute it
    while(i < tokens.size()) {
        map<string, Command*>::iterator it =
                                        commands.find(tokens[i]);
        Command* c;
        if(it != commands.end()) 
            c = it->second;
        else
            c = new AssignCommand();
        i = c->execute(i, tokens);    
    }
    
}

