#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "parser.h"
#include "../lexer/token.h"

parser_t::parser_t(std::istream& stream)
    :lexer(new lexer_t(stream)), parsed(false),
     ast_root(new astree_t(new syntaxunit_t("program"))),
     ast_current(ast_root)
{}

parser_t::~parser_t()
{
    delete lexer;
}

int parser_t::parse()
{
    return parse_expr();
}

bool check(token_t token, const std::string& cmp)
{
    return 0 == token.value.compare(cmp);
}

astree_t get_ast()
{
    if(!parsed)
    {
        parse();
    }

    return ast;
}

void parser_t::parse_expr()
{
    parse_fact();

    while(true)
    {
        token_t token = lexer->peek();
        if(check(token, "+"))
        {
            lexer->next();
            result += parse_fact();
        }
        else if(check(token, "-"))
        {
            lexer->next();
            result -= parse_fact();
        }
        else break;
    }

    return result;
}

void parser_t::parse_fact()
{
    int result = parse_pow();

    while(true)
    {
        token_t token = lexer->peek();

        if(check(token, "*"))
        {
            lexer->next();
            result *= parse_pow();
        }
        else if(check(token, "/"))
        {
            lexer->next();
            result /= parse_pow();
        }
        else break;
    }
    return result;
}

void parser_t::parse_pow()
{
    int result = parse_number();
    token_t token = lexer->peek();

    if(check(token, "^"))
    {
        lexer->next();
        result = pow(result, parse_pow());
    }
    return result;
}

void parser_t::parse_number()
{
    token_t token = lexer->peek();
    if(token.type == NUMBER)
    {
        lexer->next();
        return atoi(token.value.c_str());
    }
    else if(token.type == BRACKET && check(token, "("))
    {
        lexer->next();
        int result = parse_expr();
        token = lexer->peek();
        if(token.type == BRACKET && check(token, ")"))
        {
            lexer->next();
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
