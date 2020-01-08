#include <string>
#include <map>
#include <iostream>

class SymVar {
public:
    double value;
    std::string sim;
    bool fromSim;   // true = this var comes from simulator
    SymVar(double value, std::string sim, bool fromSim) {
        this->sim = sim;
        this->value = value;
        this->fromSim = fromSim;
    }
};

class SymbolTable {
public:
    std::map<std::string, SymVar*> table;
    SymVar* getValue(std::string name) {
        std::map<std::string, SymVar*>::iterator it =
                                        table.find(name);
        if(it != table.end()) {
            return it->second;
        }
        else
            return 0;
    }
    
    void insertNewSymbol(std::string name, SymVar* v) {
        table.insert(std::pair<std::string, SymVar*>(name, v));
    }
    
    SymVar* findBySim(std::string sim) {
        for(auto it = table.begin(); it != table.end(); it++)
            if(it->second->sim == sim)
                return it->second;
        return 0;
    }

};