#include <iostream>
#include "parser/parser.h"
#include "parser/astree.h"
#include <vector>

using std::vector;

void print(astree_t* ast)
{
    syntaxunit_t unit = ast->get_unit();
    std::cout << "<" << unit.get_name();
    if(unit.has_value())
    {
        std::cout << " value = \"" << unit.get_value() << "\"";
    }
    if(ast->is_leaf())
    {
        std::cout << "/>";
        return;
    }
    std::cout << ">";
    vector<astree_t*> childs = ast->get_childs();
    std::cout.flush();
    for(int i = 0; i < childs.size(); i++)
    {
        print(childs[i]);
    }
    std::cout << "</" << unit.get_name() << ">";
}

int main(int argc, const char *argv[])
{
    parser_t parser(std::cin);
    astree_t* ast = parser.get_ast();

    print(ast);
    return 0;
}
