#include <exception>
#include <stdexcept>
#include "astree.h"

namespace
{
    const char* NO_CHILD_ERROR = "Node doesn't have childs";
}

using std::vector;
using std::logic_error;

astree_t::astree_t(const syntaxunit_t& _unit)
    :unit(_unit)
{}

astree_t::~astree_t()
{
    for(int i = 0; i < childs.size(); i++)
    {
        delete childs[i];
    }
}

vector<astree_t*> astree_t::get_childs()
{
    if(!is_leaf())
    {
        throw logic_error(NO_CHILD_ERROR);
    }

    return childs;
}

void astree_t::add_child(astree_t* child)
{
    childs.push_back(child);
}

syntaxunit_t astree_t::get_unit()
{
    return unit;
}

bool astree_t::is_leaf()
{
    return !childs.empty();
}
