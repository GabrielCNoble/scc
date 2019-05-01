#ifndef OBJ_H
#define OBJ_H



#include "type.h"
#include "scope.h"

enum STORAGE_CLASS
{
    STORAGE_CLASS_AUTO = 0,         /* default... */
    STORAGE_CLASS_EXTERN,
    STORAGE_CLASS_STATIC,
};

struct object_t
{
    struct object_t *next;
    struct scope_t *scope;
    struct base_type_t *type;
    char *id;
    int storage_class;
    unsigned int offset;            /* offset from the segment pointer... */
};


#endif // OBJ_H
