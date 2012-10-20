#include <exception>
#include <stdexcept>
#include "syntaxunit.h"

namespace
{
    const char* NO_VALUE_ERROR = "Syntax unit doesn't have value";
}

using std::string;
using std::logic_error;

syntaxunit_t::syntaxunit_t(const string& _unit_name)
    :unit_name(_unit_name)
{}

syntaxunit_t::syntaxunit_t(const string& _unit_name,
                           const string& _value)
    :unit_name(_unit_name), value(_value)
{}

std::string& syntaxunit_t::get_value()
{
    if(has_value())
    {
        return value;
    }
    else
    {
        throw logic_error(NO_VALUE_ERROR);
    }
}
std::string& syntaxunit_t::get_name()
{
    return unit_name;
}

bool syntaxunit_t::has_value()
{
    return !value.empty();
}
