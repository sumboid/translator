#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "parser.h"
#include "syntaxtype.h"
#include "../lexer/token.h"

namespace
{
    bool check(token_t token, const std::string& cmp)
    {
        return 0 == token.value.compare(cmp);
    }
}
using std::string;

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
    while(lexer->peek().type != END_OF_FILE)
    {
        ast_root->add_child(parse_func());
    }

    parsed = true;
}

astree_t* parser_t::parse_func()
{
    if(lexer->peek().type != TYPE)
    {
        throw -1;
    }

    string type = lexer->peek().value;
    astree_t* decl_func_root = new astree_t(syntaxunit_t(syntaxtype::DECL, type));

    lexer->next();

    if(lexer->peek().type != VARIABLE)
    {
        throw -1;
    }

    string name = lexer->peek().value;
    astree_t* decl_func = new astree_t(syntaxunit_t(syntaxtype::FUNC, name));

    lexer->next();

    decl_func_root->add_child(decl_func);

    decl_func->add_child(parse_func_args());
    decl_func->add_child(parse_func_body());

    return decl_func_root;
}

astree_t* parser_t::parse_func_args()
{
    if(!check(lexer->peek(), "("))
    {
        throw -1;
    }
    lexer->next();

    astree_t* args_root = new astree_t(syntaxunit_t(syntaxtype::FUNC_ARGS));
    bool kill = false;

    while(!check(lexer->peek(), ")"))
    {
        if(kill) //XXX: :<
        {
            throw -1;
        }

        if(lexer->peek().type != TYPE)
        {
            throw -1;
        }
        astree_t* decl = new astree_t(syntaxunit_t(syntaxtype::DECL,
                    lexer->peek().value));

        lexer->next();

        if(lexer->peek().type != VARIABLE)
        {
            throw -1;
        }
        astree_t* var = new astree_t(syntaxunit_t(syntaxtype::VAR,
                    lexer->peek().value));
        decl->add_child(var);
        args_root->add_child(decl);
        lexer->next();

        if(lexer->peek().type != COMMA)
        {
            kill = true;
        }
        else
        {
            lexer->next();
        }
    }

    lexer->next();
    return args_root;
}

astree_t* parser_t::parse_func_body()
{
    if(!check(lexer->peek(), "{"))
    {
        throw -1;
    }

    lexer->next();
    astree_t* body_root = new astree_t(syntaxunit_t(syntaxtype::FUNC_BODY));
    while(!check(lexer->peek(), "}"))
    {
        astree_t* local_root;
        if(lexer->peek().type == TYPE)
        {
            string type_of_variable = lexer->peek().value;
            syntaxunit_t decl_unit(syntaxtype::DECL, type_of_variable);
            local_root = new astree_t(decl_unit);
            lexer->next();
            if(lexer->peek().type == VARIABLE)
            {
                string name_of_variable = lexer->peek().value;
                syntaxunit_t var_unit(syntaxtype::VAR, name_of_variable);
                local_root->add_child(new astree_t(var_unit));
                body_root->add_child(local_root);
                lexer->next();
                if(lexer->peek().type == DELIMITER)
                {
                    lexer->next();
                    continue;
                }
                else if(lexer->peek().type == ASSIGN)
                {
                    syntaxunit_t assign_unit(syntaxtype::ASSIGN);
                    astree_t* assign_root = new astree_t(assign_unit);
                    assign_root->add_child(new astree_t(var_unit));
                    lexer->next();
                    assign_root->add_child(parse_expr());
                    if(lexer->peek().type == DELIMITER)
                    {
                        body_root->add_child(assign_root);
                        lexer->next();
                        continue;
                    }
                }
            }
            throw  -1;
        }
        else if(lexer->peek().type == VARIABLE)
        {
            string name_of_variable = lexer->peek().value;
            syntaxunit_t var_unit(syntaxtype::VAR, name_of_variable);
            lexer->next();
            if(lexer->peek().type == DELIMITER)
            {
                ast_root->add_child(new astree_t(var_unit));
                lexer->next();
                continue;
            }
            else if(lexer->peek().type == ASSIGN)
            {
                syntaxunit_t assign_unit(syntaxtype::ASSIGN);
                astree_t* assign_root = new astree_t(assign_unit);
                assign_root->add_child(new astree_t(var_unit));
                lexer->next();
                assign_root->add_child(parse_expr());
                if(lexer->peek().type == DELIMITER)
                {
                    body_root->add_child(assign_root);
                    lexer->next();
                    continue;
                }
            }
            throw -1;
        }
        else if(lexer->peek().type == RETURN)
        {
            syntaxunit_t return_unit(syntaxtype::RETURN);
            astree_t* return_root = new astree_t(return_unit);
            body_root->add_child(return_root);
            lexer->next();
            if(lexer->peek().type != DELIMITER)
            {
                return_root->add_child(parse_expr());
                if(lexer->peek().type != DELIMITER)
                {
                    throw -1;
                }
            }

            lexer->next();
            continue;
        }
        else if(lexer->peek().type == DELIMITER)
        {
            lexer->next();
            continue;
        }
        else
        {
            throw -1;
        }
    }

    lexer->next();
    return body_root;
}


astree_t* parse_decl()
{
    astree_t* decl_root = parse_type();
    decl_root->add_child(parse_name());

    return decl_root;
}

astree_t* parse_type()
{
    token_t token = lexer->peek();
    if(token.type != TYPE)
    {
        throw logic_error(TYPE_EXPECTED_ERROR + ": \'" + token.value + "\'");
    }

    astree_t* decl = new astree_t(new syntaxunit_t(syntaxtype::DECL, token.value));
    return decl;
}

astree_t* parse_name()
{
    lexer->next();
    token_t token = lexer->peek();
    if(token.type != VARIABLE)
    {
        throw logic_error(VARIABLE_EXPECTED_ERROR + ": \'" + token.value + "\'");
    }

    astree_t* var = new astree_t(new syntaxunit_t(syntaxtype::VAR, token.value));

    return var;
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

astree_t* parser_t::parse_number() //XXX: RENAME!
{
    token_t token = lexer->peek();
    astree_t* local_root;
    if(token.type == NUMBER)
    {
        lexer->next();
        syntaxunit_t const_unit(syntaxtype::CONST, token.value);
        local_root = new astree_t(const_unit);
    }
    else if(token.type == VARIABLE)
    {
        lexer->next();
        if(check(lexer->peek(), "("))
        {
            syntaxunit_t funcall_unit(syntaxtype::FUNCALL, token.value);
            local_root = new astree_t(funcall_unit);
            local_root->add_child(parse_funcall());
        }
        else
        {
            syntaxunit_t var_unit(syntaxtype::VAR, token.value);
            local_root = new astree_t(var_unit);
        }
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

astree_t* parser_t::parse_funcall()
{
    if(!check(lexer->peek(), "("))
    {
        throw -1;
    }
    lexer->next();

    astree_t* args_root = new astree_t(syntaxunit_t(syntaxtype::FUNC_ARGS));
    bool kill = false;

    while(!check(lexer->peek(), ")"))
    {
        if(kill) //XXX: :<
        {
            throw -1;
        }

        args_root->add_child(parse_expr());

        if(lexer->peek().type != COMMA)
        {
            kill = true;
        }
        else
        {
            lexer->next();
        }
    }

    lexer->next();
    return args_root;
}
