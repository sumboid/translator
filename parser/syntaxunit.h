#ifndef SYNTAXUNIT_H
#define SYNTAXUNIT_H

#include <string>
#include "syntaxtype.h"

class syntaxunit_t
{
public:
    syntaxunit_t();
    syntaxunit_t (SyntaxunitType unit);
    syntaxunit_t (SyntaxunitType unit,
                  const std::string& value);

    std::string& get_value();
    SyntaxunitType& get_name();
    bool has_value();
private:
    SyntaxunitType unit_name;
    std::string value;
};


#endif /* end of include guard: SYNTAXUNIT_H */
