#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "../parser/astree.h"
#include "../parser/syntaxtype.h"

struct function_state_t
{
    std::map<std::string, int> variables;
    int stack_offset;
    int current_offset;
    int args_number;

    function_state_t()
    {
        stack_offset = 0;
        current_offset = 0;
    }
};

class translator_t
{
public:
    translator_t (std::ostream&);
    virtual ~translator_t ();

    void translate(astree_t*);
private:
    void translate_program(astree_t*);
    void translate_function(astree_t*);
    std::vector<std::string> translate_body(astree_t*);
    void translate_if(astree_t*);
    int  translate_condition(astree_t*);
    void translate_assign(astree_t*);
    void translate_return(astree_t*);
    void translate_expr(astree_t*);
    void push(std::string, bool);
    void pop(std::string);
    void make_operation(SyntaxunitType);

    std::map<std::string, function_state_t> functions;
    std::string current_function;

    int label_number;

    std::ostream& stream;
    std::stringstream line;
};


#endif /* end of include guard: TRANSLATOR_H */
