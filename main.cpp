#include <iostream>
#include "lexer.h"

int main(int argc, const char *argv[])
{
    lexer_t lexer(std::cin);
    token_t token = lexer.peek();
    
    while(token.type != END_OF_FILE)
    {
        std::cout << "{" << token.value << ": " << token.type << "}" << std::endl;
        lexer.next();
        token = lexer.peek();
    }
    std::cout << "{" << token.value << ": " << token.type << "}" << std::endl;

    return 0;
}
