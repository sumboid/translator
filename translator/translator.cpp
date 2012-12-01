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

    vector<astree_t*> args = tmp[1]->get_childs();
    functions[current_function].args_number = args.size();

    for(int i = 0; i < args.size(); i++)
    {
        functions[current_function].variables[args[i]->get_childs()[0]->get_unit().get_value()] = INTEGER_OFFSET * (i + 1);
    }

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
        string unit_name = current->get_unit().get_name();
        if(0 == unit_name.compare(syntaxtype::RETURN))
        {
            translate_return(current);
        }
        else if(0 == unit_name.compare(syntaxtype::DECL))
        {
            if(functions[current_function].variables.find(current->get_childs()[0]->get_unit().get_value()) !=
                    functions[current_function].variables.end())
            {
                throw 1;
            }

            push("0", false);
            functions[current_function].variables[current->get_childs()[0]->get_unit().get_value()] = 
                functions[current_function].current_offset;
        }
        else if(0 == unit_name.compare(syntaxtype::ASSIGN))
        {
            translate_assign(current);
        }
    }
}

void translator_t::translate_return(astree_t* return_root)
{
    astree_t* expression = return_root->get_childs()[0];
    translate_expr(expression);
    push("eax", true);
    line << "\nleave\nret\n";
}

void translator_t::translate_assign(astree_t* assign_root)
{
    translate_expr(assign_root->get_childs()[1]);
    pop("eax");
    line << "movl %eax, " << functions[current_function].variables[assign_root->get_childs()[0]->get_unit().get_value()] << "(%ebp)\n";
}

void translator_t::translate_expr(astree_t* expr)
{
    if(expr->is_leaf())
    {
        if(0 == expr->get_unit().get_name().compare(syntaxtype::VAR))
        {
            line << "movl " << functions[current_function].variables[expr->get_unit().get_value()] << "(%ebp), %eax\n";
            push("eax", true);
        }
        else if(0 == expr->get_unit().get_name().compare(syntaxtype::CONST))
        {
            push(expr->get_unit().get_value(), false);
        }
        else if(0 == expr->get_unit().get_name().compare(syntaxtype::FUNCALL))
        {
            string function_name = expr->get_unit().get_value();
            astree_t* args_root = expr->get_childs()[0];

            if(!args_root->is_leaf())
            {
                vector<astree_t*> args = args_root->get_childs();
                if(functions[function_name].args_number != args.size())
                {
                    throw 1;
                }

                for(int i = 0; i < args.size(); i++)
                {
                    translate_expr(args[i]);
                }

                line << "call " << function_name << "\n";
                for(int i = 0; i < args.size(); i++)
                {
                    pop("edx");
                }
            }
            else
            {
                if(0 != functions[function_name].args_number)
                {
                    throw 1;
                }
                line << "call " << function_name << "\n";
            }

            push("eax", true);
        }
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
