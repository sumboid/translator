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

    while(true)
    {
        token_t token = lexer->peek();
        syntaxunit_t current_unit;
        if(check(token, "+"))
        {
            current_unit = syntaxunit_t(syntaxtype::ADD);
        }
        else if(check(token, "-"))
        {
            current_unit = syntaxunit_t(syntaxtype::SUB);
        }
        else
        {
            break;
        }

        left = local_root;
        local_root = new astree_t(current_unit);
        local_root->add_child(left);
        lexer->next();
        astree_t* right = parse_fact();
        local_root->add_child(right);
    }

    return local_root;
}

astree_t* parser_t::parse_fact()
{
    astree_t* left = parse_pow();
    astree_t* local_root = left;

    while(true)
    {
        token_t token = lexer->peek();
        syntaxunit_t current_unit;
        if(check(token, "*"))
        {
            current_unit = syntaxunit_t(syntaxtype::MUL);
        }
        else if(check(token, "/"))
        {
            current_unit = syntaxunit_t(syntaxtype::DIV);
        }
        else
        {
            break;
        }

        left = local_root;
        local_root = new astree_t(current_unit);
        local_root->add_child(left);
        lexer->next();
        astree_t* right = parse_pow();
        local_root->add_child(right);
    }

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
        local_root->add_child(right);
        local_root->add_child(left);
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
