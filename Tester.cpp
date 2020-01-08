#include "Lexer.h"
#include "Parser.h"

int main() {
    Lexer lex("fly.txt");
    
    Parser parser(lex.tokenize());
    parser.parse();
    return 0;
}