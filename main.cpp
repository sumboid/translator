#include <iostream>
#include "parser/parser.h"
#include "parser/astree.h"
#include "translator/translator.h"
#include <vector>
#include <string>

using std::vector;

void print(astree_t* ast)
{
    if(ast == NULL)
    {
        std::cout << "NULL!" << std::endl;
    }
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
    if(argc >= 2 && std::string("-a").compare(argv[1]) == 0)
    {
        print(ast);
        return 0;
    }
    translator_t t(std::cout);
    t.translate(ast);
    return 0;
}
