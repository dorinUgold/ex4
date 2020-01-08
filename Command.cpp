#include "Command.h"
#define PORT 5400
#define PORTCLIENT 5402
std::condition_variable cv;
std::mutex m;
std::string data;
bool working = true;
SymbolTable symTable;   // symbol table for variables
string ops[] = {"==", "<", ">", ">=", "<=", "!="};
const double EPSILON = 0.00000001;
map<string, Command*> commands;

// creates a connection to the server and gets data from it
// updates the server's data to the symbol table
int createServer(int port) {
    map<int, string> simVals;
    
    // those values come from the XML file and are the
    // data sent by the server
    simVals.insert(pair<int, string>(0, "/instrumentation/airspeed-indicator/indicated-speed-kt"));
    simVals.insert(pair<int, string>(1, "/sim/time/warp"));
    simVals.insert(pair<int, string>(2, "//controls/switches/magnetos"));
    simVals.insert(pair<int, string>(3, "/instrumentation/heading-indicator/offset-deg"));
    simVals.insert(pair<int, string>(4, "/instrumentation/altimeter/indicated-altitude-ft"));
    simVals.insert(pair<int, string>(5, "/instrumentation/altimeter/pressure-alt-ft"));
    simVals.insert(pair<int, string>(6, "/instrumentation/attitude-indicator/indicated-pitch-deg"));
    simVals.insert(pair<int, string>(7, "/instrumentation/attitude-indicator/indicated-roll-deg"));
    simVals.insert(pair<int, string>(8, "/instrumentation/attitude-indicator/internal-pitch-deg"));
    simVals.insert(pair<int, string>(9, "/instrumentation/attitude-indicator/internal-roll-deg"));
    simVals.insert(pair<int, string>(10, "instrumentation/encoder/indicated-altitude-ft"));
    simVals.insert(pair<int, string>(11, "/instrumentation/encoder/pressure-alt-ft"));
    simVals.insert(pair<int, string>(12, "/instrumentation/gps/indicated-altitude-ft"));
    simVals.insert(pair<int, string>(13, "/instrumentation/gps/indicated-ground-speed-kt"));
    simVals.insert(pair<int, string>(14, "/instrumentation/gps/indicated-vertical-speed"));
    simVals.insert(pair<int, string>(15, "/instrumentation/heading-indicator/indicated-heading-deg"));
    simVals.insert(pair<int, string>(16, "/instrumentation/magnetic-compass/indicated-heading-deg"));
    simVals.insert(pair<int, string>(17, "/instrumentation/slip-skid-ball/indicated-slip-skid"));
    simVals.insert(pair<int, string>(18, "/instrumentation/turn-indicator/indicated-turn-rate"));
    simVals.insert(pair<int, string>(19, "/instrumentation/vertical-speed-indicator/indicated-speed-fpm"));
    simVals.insert(pair<int, string>(20, "/controls/flight/aileron"));
    simVals.insert(pair<int, string>(21, "/controls/flight/elevator"));
    simVals.insert(pair<int, string>(22, "/controls/flight/rudder"));
    simVals.insert(pair<int, string>(23, "/controls/flight/flaps"));
    simVals.insert(pair<int, string>(24, "/controls/engines/engine/throttle"));
    simVals.insert(pair<int, string>(25, "/controls/engines/current-engine/throttle"));
    simVals.insert(pair<int, string>(26, "/controls/switches/master-avionics"));
    simVals.insert(pair<int, string>(27, "/controls/switches/starter"));
    simVals.insert(pair<int, string>(28, "/engines/active-engine/auto-start"));
    simVals.insert(pair<int, string>(29, "/controls/flight/speedbrake"));
    simVals.insert(pair<int, string>(30, "/sim/model/c172p/brake-parking"));
    simVals.insert(pair<int, string>(31, "/controls/engines/engine/primer"));
    simVals.insert(pair<int, string>(32, "/controls/engines/current-engine/mixture"));
    simVals.insert(pair<int, string>(33, "/controls/switches/master-bat"));
    simVals.insert(pair<int, string>(34, "/controls/switches/master-alt"));
    simVals.insert(pair<int, string>(35, "/engines/engine/rpm"));
    
    //create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket"<<std::endl;
        return -1;
    }

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    //we need to convert our number
    // to a number that the network understands.

    //the actual bind command
    if (::bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr<<"Could not bind the socket to an IP"<<std::endl;
        return -2;
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr<<"Error during listening command"<<std::endl;
        return -3;
    } else{
        std::cout<<"Server is now listening ..."<<std::endl;
    }

    // accepting a client
    int client_socket = accept(socketfd,
                        (struct sockaddr*)&address,
                        (socklen_t*)&address);
    bool firstTime = true;
    while(client_socket != -1) {
        //reading from client
        char buffer[1024] = {0};
        int valread = read( client_socket , buffer, 1024);
        
        int from, to, ind;
        int last = 1023;
        while(buffer[last] != '.')
            last--;
        buffer[last+2] = ',';
        last += 2;
        from = to = ind = 0;
        while(to <= last) {
            while(buffer[to] != ',')
                to++;
            string sVal = "";
            for(int i = from; i < to; i++)
                sVal.push_back(buffer[i]);
            map<int, string>::iterator it =
                                        simVals.find(ind);

            string name = it->second;

            SymVar* v = symTable.findBySim(name);
            if(v != NULL) {
                if(v->fromSim) 
                    v->value = stod(sVal);
            }
            ind++;
            from = ++to;   
        }
        
        
        if(firstTime) {
            cv.notify_one();
            cout<<"Done"<<endl;
            firstTime = false;
        }
                    
    }
    
    close(socketfd); //closing the listening socket
    return 0;
}


void createClient() {
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) 
        std::cerr << "Could not create a socket"<<std::endl;

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  
    address.sin_port = htons(PORTCLIENT);

    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) 
        std::cerr << "Could not connect to host server"<<std::endl;
    else 
        std::cout<<"Client is now connected to server" <<std::endl;

    while(working) {
        std::unique_lock<std::mutex> lk(m);    
        cv.wait(lk);

        //if here we made a connection

        char msg[data.length() + 1]; 
        strcpy(msg, data.c_str());
        int sent = send(client_socket, msg, strlen(msg), 0);
        if (sent == -1) 
            std::cout<<"Error sending message"<<std::endl;
    }

    close(client_socket);

        
}

int OpenServerCommand::execute(int index, vector<string> tokens) {
    index += 2;
    int from = index;
    while(tokens[index] != ")")
        index++;
    double port = createExp(tokens, from, index-1);
    
    thread t1(createServer, (int)port);
    t1.detach();
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul);
    return index + 2;
}

int VarCommand::execute(int index, vector<string> tokens) {
        string name = tokens[index + 1];
        bool fromSim;
        if(tokens[index + 2] == "->")
            fromSim = false;
        else
            fromSim = true;
        string sim = "";
        for(int i = 1; i < tokens[index + 5].length() - 1; i++)
            sim.push_back(tokens[index + 5][i]);
        
        SymVar *v = new SymVar(0, sim, fromSim);
        symTable.insertNewSymbol(name, v);

        while(tokens[index] != "#")
            index++;
        return index + 1;
    }
int PrintCommand::execute(int index, vector<string> tokens) {
    index += 2;
        
    if(tokens[index][0] == '\"')
        cout<<tokens[index]<<endl;
    else {
        int from = index;
        while(tokens[index] != ")")
            index++;  
        cout<<createExp(tokens, from, index-1)<<endl;
    }
            
        
    while(tokens[index] != "#")
        index++;
    return index + 1;
}

int AssignCommand::execute(int index, vector<string> tokens) {
    if(tokens[index] == "#")
        return index + 1;
    SymVar* v = symTable.getValue(tokens[index]);
    
    Interpreter inter;
    index += 2;
    if(tokens[index+1] == "#") 
        v->value = stod(tokens[index]);
    else {
        int from = index;
        while(tokens[index] != "#") 
            index++;
        v->value = createExp(tokens, from, index-1);
    }
    // send data to server
    data = "set " + v->sim + " " + to_string(v->value);
    cv.notify_one();
    
        
    while(tokens[index] != "#") {
            index++;
    }
        return index + 1;
}

double createExp(vector<string> tokens, int from, int to) {
    Interpreter inter;
    string exp = "";

    for(int i = from; i <= to; i++) {
        if(isalpha(tokens[i][0])) {
            SymVar* var = symTable.getValue(tokens[i]); 
            double val = symTable.getValue(tokens[i])->value;
            inter.setVariables(tokens[i] + "=" + to_string(val));
        }
        exp += tokens[i];
    }
    
    return inter.interpret(exp);
}

int IfCommand::execute(int index, vector<string> tokens) {
    if(commands.size() == 0)
        createCommands();
    
    index += 1;
    
    int from = index;
    while(!isBooleanOp(tokens[index]))
        index++;
    double left = createExp(tokens, from, index - 1);
    string op = tokens[index++];
    from = index;
    while(tokens[index] != "{")
        index++;
    double right = createExp(tokens, from, index - 1);
  
    bool res;
    res = evaluateCondition(left, right, op);
    
    if(res) {
        index += 2;
        
        while(tokens[index] != "}") {
            map<string, Command*>::iterator it =
                         commands.find(tokens[index]);
            Command* c;
            if(it != commands.end()) 
                c = it->second;
            else
                c = new AssignCommand();
            index = c->execute(index, tokens);    
        }
    }
    while(tokens[index] != "}")
        index++;
    return index + 2;
}

int WhileCommand::execute(int index, vector<string> tokens) {
       if(commands.size() == 0)
        createCommands();
    
    index += 1;
    
    int leftFrom = index;
    while(!isBooleanOp(tokens[index]))
        index++;
    int leftTo = index-1;
    double left = createExp(tokens, leftFrom, leftTo);
    string op = tokens[index++];
    int rightFrom = index;
    while(tokens[index] != "{")
        index++;
    int rightTo = index - 1;
    double right = createExp(tokens, rightFrom, rightTo);
  
    bool res;
    res = evaluateCondition(left, right, op);
    index += 2;
    int startWhile = index;
    while(res) {
        index = startWhile;
        
        while(tokens[index] != "}") {
            map<string, Command*>::iterator it =
                         commands.find(tokens[index]);
            Command* c;
            if(it != commands.end()) 
                c = it->second;
            else
                c = new AssignCommand();
            index = c->execute(index, tokens);    
        }
        left = createExp(tokens, leftFrom, leftTo);
        right = createExp(tokens, rightFrom, rightTo);
        
        res = evaluateCondition(left, right, op);
    }
    while(tokens[index] != "}")
        index++;
    return index + 2;
    }

bool isBooleanOp(string s) {
    for(int i = 0; i < 6; i++)
        if(ops[i] == s)
            return true;
    return false;
}

bool evaluateCondition(double left, double right, string op) {
    if(op == "==") 
        return fabs(left - right) <= EPSILON * fabs(left);
    if(op == "<")
        return left < right;
    if(op == ">")
        return left > right;
    if(op == ">=")
        return left >= right;
    if(op == "<=")
        return left <= right;
    if(op == "!=")
        return fabs(left - right) > EPSILON * fabs(left);
    return false;
}

void createCommands() {
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