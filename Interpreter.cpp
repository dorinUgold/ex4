#include "Interpreter.h"

void Interpreter::createTokens(string exp) {
    exp = "(" + exp + ")";
    
    int i = 0, state = 0, start, end;
    while(i < exp.length()) {
        char c = exp[i];
        switch(state) {
        case 0:
            if(c == '(') {
                state = 1;
                tokens.push("(");
            }
            break;
        case 1:
            if(c == '-' || c == '+') {
                string s = "  ";
                s[0] = c;
                s[1] = c;
                tokens.push(s);
                state = 2;
            }
            else if(isalpha(c) || c == '_') {
                start = i;
                state = 4;
            }
            else if(isdigit(c) || c == '.') {
                start  = i;
                state = 3;
            }
            else if(c == '(')
                state = 1;
            else
                throw "bad input";
            break;
        case 2:
            if(c == '(')
                state = 1;
            else if(isalpha(c) || c == '_') {
                start = i;
                state = 4;
            }
            else if(isdigit(c) || c == '.') {
                start = i;
                state = 3;
            }
            else
                throw "bad input";
            break;
        case 3:
            if(isdigit(c) || c == '.') {
                state = 3;
            }
            else if(c == '+' || c == '-' || c == '*' || c =='/'){
                string num = exp.substr(start, (i-start));
                tokens.push(num);
                string s = " ";
                s[0] = c;
                tokens.push(s);
                state = 5;
            }
            else if(c == ')') {
                string num = exp.substr(start, (i-start));
                tokens.push(num);
                tokens.push(")");
                state = 6;
            }
            else
                throw "bad input";
            break;
        case 4:
            if(isalpha(c) || c == '_' || isdigit(c)) {
                state = 4;
            }
            else if(c == '+' || c == '-' || c == '*' || c =='/'){
                string num = exp.substr(start, (i-start));
                tokens.push(num);
                string s = " ";
                s[0] = c;
                tokens.push(s);
                state = 5;
            }
            else if(c == ')') {
                string num = exp.substr(start, (i-start));
                tokens.push(num);
                tokens.push(")");
                state = 6;
            }
            else
                throw "bad input";
            break;
        case 5:
            if(isalpha(c) || c == '_') {
                start = i;
                state = 4;
            }
            else if(isdigit(c) || c == '.') {
                start = i;
                state = 3;
            }
            else if(c == '(')
                state = 1;
            else
                throw "bad input";
            break;
        case 6:
            if(c == '+' || c == '-' || c == '*' || c =='/'){
                string s = " ";
                s[0] = c;
                tokens.push(s);
                state = 5;
            }
            else if(c == ')') {
                tokens.push(")");
                state = 6;
            }
            else
                throw "bad input";
            break;
        }
        i++;
    }
   
    
}


void Interpreter::setVariables(string s) {
    int i = 0, start;
    
    while(i < s.length()) {
        start = i;
        while(s[i] != '=')
            i++;
        string name = s.substr(start, i-start);
        i++;
        start = i;
        while(i < s.length() && s[i] != ';')
            i++;
        string val = s.substr(start, i-start);
        i++;
        
        try {
            vars[name] = stod(val);
        }
        catch(std::invalid_argument e) {
            throw "bad input";
        }
    }
    
}

double Interpreter::interpret(string exp) {
    
    createTokens(exp);
    
    if(tokens.size() == 2) {
        if(isalpha(tokens.front()[0]))
            return vars.find(tokens.front())->second;
        return stod(tokens.front());
    }
    stack<string> ops;
    stack<double> vals;
    
    while(!tokens.empty()) {
       string s = tokens.front();
        tokens.pop();
        cout<<s<<endl;
        if(s == "+" || s == "-" || s == "*" || s == "/" ||
                s == "--" || s == "++")
           ops.push(s);
        else if(isdigit(s[0]))
            vals.push(stod(s));
        else if(vars.find(s) != vars.end()) {
            vals.push(vars.find(s)->second);
        }
        else if(s == "(")
            ops.push(s);
        else if(s == ")") {
            while(ops.top() != "(") {
                double v1, v2, res;
                string op = ops.top();
                ops.pop();
                if(op == "--" || op == "++") {
                    res = vals.top();
                    vals.pop();
                    if(op == "--")
                        res *= -1;
                }
                else {
                    v1 = vals.top();
                    vals.pop();
                    v2 = vals.top();
                    vals.pop();
                    if(op == "+")
                        res = v1 + v2;
                    else if(op == "-")
                        res = v2 - v1;
                    else if(op == "*")
                        res = v1 * v2;
                    else if(op == "/")
                        res = v2 / v1;
                }
                vals.push(res);
               
            }
            ops.pop();
        }
        
       
    }
    cout<<vals.top();
    return vals.top();
    
   
}