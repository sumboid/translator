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

    astree_t* get_ast();
private:
    astree_t* parse_expr();
    astree_t* parse_fact();
    astree_t* parse_pow();
    astree_t* parse_number();
    void parse();

    lexer_t* lexer;
    astree_t* ast_root;
    bool parsed;
};
#endif /* end of include guard: PARSER_H */
