#include <exception>
#include "astree.h"

namespace
{
    const char* NO_CHILD_ERROR = "Node doesn't have childs";
    const char* NO_UNIT_ERROR = "Node doesn't have syntax unit";
}

using std::vector;
using std::logic_error;

astree_t::astree_t()
    :unit_setted(false)
{}

astree_t::astree_t(const syntaxunit_t& _unit)
    :unit(_unit), unit_setted(true)
{}

astree_t::~astree_t()
{
    for(int i = 0; i < childs.size(); i++)
    {
        delete childs;
    }
}

std::vector<astree_t*> astree_t::get_childs()
{
    if(!is_leaf())
    {
        throw logic_error(NO_CHILD_ERROR);
    }

    return childs;
}

void astree_t::add_child(const astree_t* child)
{
    child->previous = this;
    childs.push_back(child);
}

syntaxunit_t astree_t::get_unit()
{
    if(has_unit())
    {
        return unit;
    }
    else
    {
        throw logic_error(NO_UNIT_ERROR);
    }
}

bool astree_t::has_unit()
{
    return unit_setted;
}

void astree_t::set_unit(const syntaxunit_t& _unit)
{
    unit_setted = true;
    unit = _unit;
}

bool astree_t::is_leaf()
{
    return !childs.empty();
}

void swap() //XXX //XXX //XXX //XXX //XXX
{
    if(!has_unit())
    {
        throw logic_error(NO_UNIT_ERROR);
    }

    syntaxunit_t* parent = previous;
    childs = parent->childs;

    vector<syntaxunit_t>::iterator it;
    pparent = parent->previous;
    it = find(pparent->childs.begin(), pparent->childs.end(), parent);
    *it = this;

    childs.push_back(parent);

    for(int i = 0; i < childs.size(); i++)
    {
        childs[i]->previous = this;
    }
}
