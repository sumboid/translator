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
    :stream(_stream), stack_offset(0), current_offset(0)
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
    stream << ".globl main\n";
    vector<astree_t*> functions = root->get_childs();
    for(int i = 0; i < functions.size(); ++i)
    {
        translate_function(functions[i]);
    }
}

void translator_t::translate_function(astree_t* function_root)
{
    vector<astree_t*> tmp = function_root->get_childs();

    stream << tmp[0]->get_unit().get_value() << ":\n";
    stream << "pushl %ebp\n";
    stream << "movl %esp, %ebp\n\n";
    translate_body(tmp[2]);
    stream << "addl $" << stack_offset << ", %esp\n";
    stream << line.str();
    stream << "\nleave\nret\n";
}

void translator_t::translate_body(astree_t* body_root)
{
    vector<astree_t*> tmp = body_root->get_childs()[0]->get_childs();

    translate_expr(tmp[0]);
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
    current_offset -= INTEGER_OFFSET;
    if(current_offset < stack_offset)
    {
        stack_offset -= INTEGER_OFFSET;
    }

    if(!reg)
    {
        line << "movl $" << value << ", " << current_offset << "(%ebp)\n";
    }
    else
    {
        line << "movl %" << value << ", " << current_offset << "(%ebp)\n";
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
        line << "idivl " << current_offset << "(%ebp)\n";
        pop("edx");
        push("eax", true);
        return;
    }

    line << "%edx, %eax\n";
    push("eax", true);
}

void translator_t::pop(string reg)
{
    line << "movl "<< current_offset << "(%ebp), %" << reg << "\n";
    current_offset += INTEGER_OFFSET;
}
