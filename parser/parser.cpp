#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "parser.h"
#include "syntaxtype.h"
#include "../lexer/token.h"

parser_t::parser_t(std::istream& stream)
    :lexer(new lexer_t(stream)), parsed(false)
{}

parser_t::~parser_t()
{
    delete lexer;
    delete ast_root;
}

void parser_t::parse()
{
    ast_root = new astree_t(syntaxunit_t("program"));
    ast_root->add_child(parse_expr());
    parsed = true;
}

bool check(token_t token, const std::string& cmp)
{
    return 0 == token.value.compare(cmp);
}

astree_t* parser_t::get_ast()
{
    if(!parsed)
    {
        parse();
    }

    return ast_root;
}

astree_t* parser_t::parse_expr()
{
    astree_t* left = parse_fact();
    astree_t* local_root = left;
    token_t token = lexer->peek();

    if(check(token, "+"))
    {
        std::cout << "+" << std::endl;
        syntaxunit_t add_unit(syntaxtype::ADD);
        local_root = new astree_t(add_unit);
    }
    else if(check(token, "-"))
    {
        syntaxunit_t sub_unit(syntaxtype::SUB);
        local_root = new astree_t(sub_unit);
    }
    else
    {
        return local_root;
    }

    local_root->add_child(left);
    lexer->next();
    astree_t* right = parse_expr();
    local_root->add_child(right);

    return local_root;
}

astree_t* parser_t::parse_fact()
{
    astree_t* left = parse_pow();
    astree_t* local_root = left;
    token_t token = lexer->peek();

    if(check(token, "*"))
    {
        syntaxunit_t mul_unit(syntaxtype::MUL);
        local_root = new astree_t(mul_unit);
    }
    else if(check(token, "/"))
    {
        syntaxunit_t div_unit(syntaxtype::DIV);
        local_root = new astree_t(div_unit);
    }
    else
    {
        return local_root;
    }

    local_root->add_child(left);
    lexer->next();
    astree_t* right = parse_fact();
    local_root->add_child(right);

    return local_root;
}

astree_t* parser_t::parse_pow()
{
    astree_t* right = parse_number();
    astree_t* local_root = right;
    token_t token = lexer->peek();

    if(check(token, "^"))
    {
        syntaxunit_t pow_unit(syntaxtype::POW);
        local_root = new astree_t(pow_unit);
        lexer->next();
        astree_t* left = parse_pow();
        local_root->add_child(left);
        local_root->add_child(right);
    }
    return local_root;
}

astree_t* parser_t::parse_number()
{
    token_t token = lexer->peek();
    astree_t* local_root;
    if(token.type == NUMBER)
    {
        lexer->next();
        syntaxunit_t const_unit(syntaxtype::CONST, token.value);
        local_root = new astree_t(const_unit);
    }
    else if(token.type == BRACKET && check(token, "("))
    {
        lexer->next();
        local_root = parse_expr();
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

    return local_root;
}
