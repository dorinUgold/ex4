#include "Lexer.h"

Lexer::Lexer(string fPath) {
    this->fPath = fPath;
}

// Tokenize a string into a vector of strings
// for each line - split into tokens
vector<string> Lexer::tokenize() {
    ifstream in {fPath, std::ios::in};
    string line;
    
    while(!in.eof()) {
        getline(in, line);
        splitLine(line);
    }
    
    return tokens;
}

void Lexer::splitLine(string line) {
    vector<string> words;
    
    // trim spaces
    size_t startpos = line.find_first_not_of(" \t");
    if( string::npos != startpos )
        line = line.substr( startpos );
    
    // finite automate for tokenizing
    line = line + "#"; // add an artificial line ending
    int state = 0, i = 0, start;
    int counter = 0;
    while(i < line.length() && counter < 1000) {
        char c = line[i];
        
        switch(state) {
        case 0:
            if(isalpha(c) || c == '_') {
                start = i;
                state = 1;  // scan word
            }
            else if(c == '(' || c == ')' || c == '{' || c =='}' 
                   || c == '*' || c == '/' || c == '+'){
                string s = " ";
                s[0] = c;
                tokens.push_back(s);
            }
            else if(c == '\"') {
                start = i;
                state = 2;
            }
            else if(c == '-') {
                start = i;
                state = 3;
            }
            else if(c == '<') {
                start = i;
                state = 4;
            }
            else if(isdigit(c)) {
                start = i;
                state = 5;
            }
            else if(c == '=' || c == '>' || c == '!') {
                if(i + 1 < line.length() && line[i+1] == '=') {
                    string s = " =";
                    s[0] = c;
                    tokens.push_back(s);
                    i++;
                }
                else {
                    string s = " ";
                    s[0] = c;
                    tokens.push_back(s);
                }
            }
            else if(c == '#')
                tokens.push_back("#");
            break;
        case 1:
            if(!isalpha(c) && c != '_' && !isdigit(c)) {
                state = 0;
                string t = line.substr(start, i-start);
                tokens.push_back(t);
                i--;
            }
            break;
        case 2:
            if(c == '\"') {
                state = 0;
                string t = line.substr(start, i-start + 1);
                tokens.push_back(t);
            }
            break;
        case 3:
            if(c == '>') 
                tokens.push_back("->");
            else {
                tokens.push_back("-");
                i--;
            }
            state = 0;
            break;
        case 4:
            if(c == '-') 
                tokens.push_back("<-");
            else if(c == '=')
                tokens.push_back("<=");
            else {
                tokens.push_back("<");
                i--; 
            }
            state = 0;
            break;
        case 5:
            if(!isdigit(c) && c != '.') {
                state = 0;
                string t = line.substr(start, i-start);
                tokens.push_back(t);
                i--;
            }
            break;
        
        }
        
        
        i++;
        counter++;
    }
    
    if(counter == 1000)
        cout<<"endless"<<endl;
    
}

