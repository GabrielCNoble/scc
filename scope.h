#ifndef SCOPE_H
#define SCOPE_H


#include "obj.h"

struct scope_t
{
    struct scope_t *parent;

    struct object_t *objects;
    struct object_t *last_object;
};


#endif // SCOPE_H
