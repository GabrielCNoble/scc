#ifndef OBJ_H
#define OBJ_H



#include "type.h"
#include "scope.h"


struct object_t
{
    struct object_t *next;
    struct scope_t *scope;
    struct base_type_t *type;
    char *id;
};


#endif // OBJ_H
