#ifndef PARSER_H
#define PARSER_H

#include <istream>
#include <vector>
#include <string>
#include "../lexer/lexer.h"
#include "astree.h"

class parser_t
{
public:
    parser_t (std::istream& stream);
    ~parser_t();

    astree_t get_ast();
private:
    void parse_expr();
    void parse_fact();
    void parse_pow();
    void parse_number();
    void parse();

    std::vector<std::string> tree;
    lexer_t* lexer;
    astree_t* ast_root;
    astree_t* ast_current;
    bool parsed;
};
#endif /* end of include guard: PARSER_H */
