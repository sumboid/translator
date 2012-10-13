#ifndef ASTREE_H
#define ASTREE_H

#include <vector>
#include "syntaxunit.h"

class astree_t
{
public:
    astree_t();
    astree_t(const syntaxunit_t&);
    virtual ~astree_t();

    std::vector<astree_t> get_childs();
    void add_child(astree_t* child);
    void add_unit(const syntaxunit_t&);
    syntaxunit_t get_unit();
    bool is_leaf();
private:
    std::vector<astree_t*> childs;
    astree_t* previous;
    syntaxunit_t unit;
};

#endif /* end of include guard: ASTREE_H */
