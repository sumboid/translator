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
    astree_t* parse_func();
    astree_t* parse_func_args();
    astree_t* parse_body();
    astree_t* parse_return();
    astree_t* parse_assign();
    astree_t* parse_assign(astree_t* var);
    astree_t* parse_decl();
    astree_t* parse_type();
    astree_t* parse_name();
    astree_t* parse_expr();
    astree_t* parse_fact();
    astree_t* parse_pow();
    astree_t* parse_number();
    astree_t* parse_funcall();
    astree_t* parse_if();
    astree_t* parse_while();
    astree_t* parse_condition();

    bool check_delimiter();

    void parse();

    lexer_t* lexer;
    astree_t* ast_root;
    bool parsed;
};
#endif /* end of include guard: PARSER_H */
