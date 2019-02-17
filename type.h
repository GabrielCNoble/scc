#ifndef TYPE_H
#define TYPE_H



enum TYPE
{
    TYPE_INT = 0,
    TYPE_SHORT,
    TYPE_CHAR,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG,
    TYPE_UNSIGNED,
    TYPE_SIGNED,
    TYPE_VOID,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_POINTER,
    TYPE_FUNCTION,
    TYPE_ARRAY,
    TYPE_IDENTIFIER,             /* this is only here to make the implementation easier... */
    TYPE_UNKNOWN,
};


struct base_type_t
{
    struct base_type_t *next;
    int type;
};

struct array_type_t
{
    struct base_type_t base;
    struct base_type_t *base_type;
    int size;
};

struct aggretage_type_t
{
    struct base_type_t base;
    struct base_type_t *fields;
};

struct function_type_t
{
    struct base_type_t base;
    struct base_type_t *params;
};

struct identifier_type_t
{
    struct base_type_t base;
    char *identifier;
};


#endif // TYPE_H






