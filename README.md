# ex3

Readme - Ex3 - Flight Simulator
-------------------------------

Compiling an Running
--------------------

The Tester.cpp file contains the main function.
The main perfomrs the following steps - 
1. Tokenize the input file (Called "fly.txt" but can be changed).
2. Create the Parser object.
3. Parse the code.

In order to compile the project the following files are needed:
Lexer.h
Lexer.cpp
Parser.h
Parser.cpp
Command.h
Command.cpp
SymbolTable.h
Interpreter.h
Interpreter.cpp
Tester.cpp

I've used clang++ compiler, you can use this line - 

clang++ Lexer.h Lexer.cpp Parser.h Parser.cpp Command.h Command.cpp SymbolTable.h Interpreter.h Interpreter.cpp Tester.cpp -std=c++11

All output is done on the terminal.

The main structures
-------------------
This project is implemented using the following structures:
1. Lexer - The Lexer class accepts a text file and tokenizes it into a vector of tokens.
2. Parser - The parser accepts the tokens vector and parse them. The parser recognizes the commands and invoke each command by its order of occurrence.
3. Command - The command class is a base virtual class to all command and implements the Command design pattern. Contains a virtual execute function.
4. Command derived classes - for each command in the simulator programming language there is a specific command class defined, e.g PrintCommand, OpenServerCommand, WhileCommand etc.
Every command overrides the execute function. This function accepts the tokens vector and execute the specific command. The function returns the current position of the index in the tokens vector.
5. SymTable - a class implements a simple symbol table for saving an administrating variables in the program.
6. Interpreter - this class omplements the shunting yard algorithm for evaluating arithmetic expressions.
