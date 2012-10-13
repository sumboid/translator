#include <iostream>
#include "parser/parser.h"

int main(int argc, const char *argv[])
{
    parser_t parser(std::cin);
    std::cout << parser.parse() << std::endl;
    //parser.parse();
    return 0;
}
