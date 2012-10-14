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

Tree* parser_t::parse()
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
            syntaxunit_t add_unit = new syntaxunit_t(ADD);
            ast_current->add_child(add_unit);
            ast_current = add_unit;
            ast_current->swap();
            lexer->next();
            parse_fact();
        }
        else if(check(token, "-"))
        {
            syntaxunit_t sub_unit = new syntaxunit_t(SUB);
            ast_current->add_child(sub_unit);
            ast_current = sub_unit;
            ast_current->swap();
            lexer->next();
            parse_fact();
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
            syntaxunit_t mul_unit = new syntaxunit_t(MUL);
            ast_current->add_child(mul_unit);
            ast_current = mul_unit;
            ast_current->swap();
            lexer->next();
            parse_pow();
        }
        else if(check(token, "/"))
        {
            syntaxunit_t div_unit = new syntaxunit_t(DIV);
            ast_current->add_child(div_unit);
            ast_current = div_unit;
            ast_current->swap();
            lexer->next();
            parse_pow();
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
        ast_current->add_child(new syntaxunit_t(NUMBER, token.value));
    }
    else if(token.type == BRACKET && check(token, "("))
    {
        lexer->next();
        parse_expr();
        token = lexer->peek();
        if(token.type == BRACKET && check(token, ")"))
        {
            lexer->next();
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

void add_
