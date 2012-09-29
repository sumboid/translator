#include "parser.h"
#include "token.h"

parser_t::parser_t(std::istream& stream)
    :lexer(new lexer_t(stream))
{}

parser_t::~parser_t()
{
    delete lexer;
}

int parser_t::parse()
{
    return parse_expr();
}

void parser_t::parse_number()
{
    token_t token = lexer->peek();
    if(token.type == NUMBER)
    {
        lexer->next();
        return atoi(token.value.c_str());
    }
    else if(token.type == BRACKET && token.value.compare("(") == 0)
    {
        lexer->next();
        int result = parse_expr();
        token = lexer->peek();
        if(token.type == BRACKET && token.value.compare(")") == 0)
        {
            return result;
        }
        else
        {
            throw -1;
        }
    }
    else
    {
        throw -1;
    }
}
