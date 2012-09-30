#include <iostream>
#include "parser.h"

int main(int argc, const char *argv[])
{
    parser_t parser(std::cin);
    std::cout << parser.parse() << std::endl;
    return 0;
}
