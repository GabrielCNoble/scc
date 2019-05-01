#ifndef FUNC_H
#define FUNC_H



#include "type.h"
#include "bc.h"


struct function_t
{
    struct object_t *object;
    struct bytecode_buffer_t *bytecode;
};



#endif // FUNC_H
