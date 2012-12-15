#include <iostream>
#include <string>
#include <cmath>
#include <string>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include "parser.h"
#include "syntaxtype.h"
#include "../lexer/token.h"

using std::logic_error;
using std::string;
namespace
{
    const string BRACKET_EXPECTED_ERROR =  "Bracket was expected";
    const string COMMA_EXPECTED_ERROR = "Comma was expected";
    const string WTF_ERROR = "What da fuck";
    const string ASSIGN_EXPECTED_ERROR = "Assign was expected";
    const string TYPE_EXPECTED_ERROR = "Name of type was expected";
    const string VARIABLE_EXPECTED_ERROR = "Name of variable was expected";


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
    ast_root = new astree_t(syntaxunit_t(S_PROGRAM));
    while(lexer->peek().type != END_OF_FILE)
    {
        ast_root->add_child(parse_func());
    }

    parsed = true;
}

astree_t* parser_t::parse_func()
{
    astree_t* decl_func_root = parse_decl();
    decl_func_root->add_child(parse_func_args());
    decl_func_root->add_child(parse_body());

    return decl_func_root;
}

astree_t* parser_t::parse_func_args()
{
    if(!check(lexer->peek(), "("))
    {
        throw logic_error(BRACKET_EXPECTED_ERROR);
    }
    lexer->next();

    astree_t* args_root = new astree_t(syntaxunit_t(S_FUNC_ARGS));
    bool kill = false;

    while(true)
    {

        astree_t* child = parse_decl();
        if(child != NULL)
            args_root->add_child(child);

        if(check(lexer->peek(), ")"))
        {
            break;
        }

        if(lexer->peek().type != COMMA)
        {
            throw logic_error(COMMA_EXPECTED_ERROR);
        }
        lexer->next();
    }

    lexer->next();
    return args_root;
}

astree_t* parser_t::parse_body()
{
    if(!check(lexer->peek(), "{"))
    {
        throw logic_error(BRACKET_EXPECTED_ERROR);
    }

    lexer->next();
    astree_t* body_root = new astree_t(syntaxunit_t(S_FUNC_BODY));

    while(!check(lexer->peek(), "}"))
    {
        astree_t* child;
        if((child = parse_decl()) != 0)
        {
            body_root->add_child(child);
            if(!check_delimiter())
            {
                child = parse_assign(new astree_t(*child->get_childs()[0])); //XXX: LEGSHOT
                body_root->add_child(child);
            }
        }
        else if((child = parse_assign()) != 0)
        {
            body_root->add_child(child);
        }
        else if((child = parse_return()) != 0)
        {
            body_root->add_child(child);
        }
       // else if((child = parse_if()) != 0)
       // {
       //     body_root->add_child(child);
       // }
        else
        {
            throw logic_error(WTF_ERROR);
        }
        lexer->next(); //Drop delimiter
    }

    lexer->next();
    return body_root;
}

astree_t* parser_t::parse_return()
{
    token_t return_token = lexer->peek();
    if(return_token.type != RETURN)
    {
        return 0;
    }
    lexer->next();

    astree_t* return_root = new astree_t(syntaxunit_t(S_RETURN));
    if(lexer->peek().type == DELIMITER)
    {
        return return_root;
    }

    return_root->add_child(parse_expr());
    return return_root;
}

astree_t* parser_t::parse_if()
{
    token_t return_token = lexer->peek();
    if(return_token.type != IF)
    {
        return 0;
    }
    lexer->next();

    astree_t* return_root = new astree_t(syntaxunit_t(S_IF));
    lexer->next();
    if(!check(lexer->peek(), "("))
    {
        throw logic_error(BRACKET_EXPECTED_ERROR);
    }
    lexer->next();
    astree_t* condition = parse_condition();
    if(condition == 0)
    {
        throw logic_error("PARSE ERROR");
    }

    if(!check(lexer->peek(), ")"))
    {
        throw logic_error(BRACKET_EXPECTED_ERROR);
    }
    lexer->next();


    return_root->add_child(parse_expr());
    return return_root;
}

astree_t* parser_t::parse_condition()
{
    return 0;
}


bool parser_t::check_delimiter()
{
    if(lexer->peek().type == DELIMITER)
    {
        return true;
    }
    return false;
}

astree_t* parser_t::parse_assign()
{
    astree_t* var;

    try
    {
        var = parse_name();
    }
    catch(const logic_error& error)
    {
        return 0;
    }

    return parse_assign(var);
}

astree_t* parser_t::parse_assign(astree_t* var)
{
    token_t assign = lexer->peek();
    if(assign.type != ASSIGN)
    {
        throw logic_error(ASSIGN_EXPECTED_ERROR + ": \'" + assign.value + "\'");
    }

    lexer->next();

    astree_t* assign_root = new astree_t(syntaxunit_t(S_ASSIGN));
    assign_root->add_child(var);
    assign_root->add_child(parse_expr());
    return assign_root;
}

astree_t* parser_t::parse_decl()
{
    astree_t* decl_root;
    try
    {
        decl_root = parse_type();
    }
    catch(const logic_error& error)
    {
        return 0;
    }

    decl_root->add_child(parse_name());

    return decl_root;
}

astree_t* parser_t::parse_type()
{
    token_t token = lexer->peek();
    if(token.type != TYPE)
    {
        throw logic_error(TYPE_EXPECTED_ERROR + ": \'" + token.value + "\'");
    }

    astree_t* decl = new astree_t(syntaxunit_t(S_DECL, token.value));
    lexer->next();
    return decl;
}

astree_t* parser_t::parse_name()
{
    token_t token = lexer->peek();
    if(token.type != VARIABLE)
    {
        throw logic_error(VARIABLE_EXPECTED_ERROR + ": \'" + token.value + "\'");
    }

    astree_t* var = new astree_t(syntaxunit_t(S_VAR, token.value));
    lexer->next();
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
            current_unit = syntaxunit_t(S_ADD);
        }
        else if(check(token, "-"))
        {
            current_unit = syntaxunit_t(S_SUB);
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
            current_unit = syntaxunit_t(S_MUL);
        }
        else if(check(token, "/"))
        {
            current_unit = syntaxunit_t(S_DIV);
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
        syntaxunit_t pow_unit(S_POW);
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
        syntaxunit_t const_unit(S_CONST, token.value);
        local_root = new astree_t(const_unit);
    }
    else if(token.type == VARIABLE)
    {
        lexer->next();
        if(check(lexer->peek(), "("))
        {
            syntaxunit_t funcall_unit(S_FUNCALL, token.value);
            local_root = new astree_t(funcall_unit);
            local_root->add_child(parse_funcall());
        }
        else
        {
            syntaxunit_t var_unit(S_VAR, token.value);
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

    astree_t* args_root = new astree_t(syntaxunit_t(S_FUNC_ARGS));
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
