#include "translator.h"
#include "../parser/astree.h"
#include "../parser/syntaxunit.h"
#include "../parser/syntaxtype.h"

#include <iostream>
#include <vector>
#include <string>
/* offset table
 * vars on stack
 * only int == 4
 * only one function
 *
 *
 */
namespace
{
    const int INTEGER_OFFSET = 4;
}

using std::ostream;
using std::vector;
using std::string;

translator_t::translator_t(ostream& _stream)
    :stream(_stream)
{}

translator_t::~translator_t()
{

}

void translator_t::translate(astree_t* root)
{
    translate_program(root);
}

void translator_t::translate_program(astree_t* root)
{
    vector<astree_t*> functions = root->get_childs();
    for(int i = 0; i < functions.size(); ++i)
    {
        translate_function(functions[i]);
    }
}

void translator_t::translate_function(astree_t* function_root)
{
    vector<astree_t*> tmp = function_root->get_childs();

    string function_name = tmp[0]->get_unit().get_value();

    current_function = function_name;
    functions[function_name] = function_state_t();

    stream << ".globl " << function_name << "\n";
    stream << function_name << ":\n";
    stream << "pushl %ebp\n";
    stream << "movl %esp, %ebp\n\n";

    translate_body(tmp[2]);
    stream << line.str();
    line.str("");
}

void translator_t::translate_body(astree_t* body_root)
{
    vector<astree_t*> body = body_root->get_childs();

    for(int i = 0; i < body.size(); i++)
    {
        astree_t* current = body[i];
        if(0 == current->get_unit().get_name().compare(syntaxtype::RETURN))
        {
            translate_return(current);
        }
    }
}

void translator_t::translate_return(astree_t* return_root)
{
    astree_t* expression = return_root->get_childs()[0];
    translate_expr(expression);
    push("eax", true);
    stream << "\nleave\nret\n";
}

void translator_t::translate_expr(astree_t* expr)
{
    if(expr->is_leaf())
    {
        push(expr->get_unit().get_value(), false);
        return;
    }

    vector<astree_t*> childs = expr->get_childs();
    translate_expr(childs[1]);
    translate_expr(childs[0]);
    make_operation(expr->get_unit().get_name());
    return;
}

void translator_t::push(string value, bool reg)
{
    functions[current_function].current_offset -= INTEGER_OFFSET;
    if(functions[current_function].current_offset < functions[current_function].stack_offset)
    {
        functions[current_function].stack_offset -= INTEGER_OFFSET;
    }

    if(!reg)
    {
        line << "movl $" << value << ", " << functions[current_function].current_offset << "(%ebp)\n";
    }
    else
    {
        line << "movl %" << value << ", " << functions[current_function].current_offset << "(%ebp)\n";
    }

}

void translator_t::make_operation(string operation)
{
    pop("eax");
    pop("edx");

    if(0 == operation.compare(syntaxtype::ADD))
    {
        line << "addl ";
    }
    if(0 == operation.compare(syntaxtype::SUB))
    {
        line << "subl ";
    }
    if(0 == operation.compare(syntaxtype::MUL))
    {
        line << "imull ";
    }
    if(0 == operation.compare(syntaxtype::DIV))
    {
        push("edx", true);
        line << "movl %eax, %edx\n";
        line << "sarl $31, %edx\n";
        line << "idivl " << functions[current_function].current_offset << "(%ebp)\n";
        pop("edx");
        push("eax", true);
        return;
    }

    line << "%edx, %eax\n";
    push("eax", true);
}

void translator_t::pop(string reg)
{
    line << "movl "<< functions[current_function].current_offset << "(%ebp), %" << reg << "\n";
    functions[current_function].current_offset += INTEGER_OFFSET;
}
