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
    TYPE_TYPEDEF,
    TYPE_FUNCTION,
    TYPE_ARRAY,
    TYPE_REFERENCE,              /* this is only here to make the implementation easier...  */
    TYPE_IDENTIFIER,             /* this is only here to make the implementation easier... */
    TYPE_UNKNOWN,
};

struct base_type_t
{
    struct base_type_t *next;
    unsigned short type;
    unsigned char is_const;
    unsigned char is_restrict;
    unsigned char is_volatile;
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
    struct reference_type_t *fields;
    char *name;
};

struct function_type_t
{
    struct base_type_t base;
    struct reference_type_t *params;
};

/* not really a type, but is here
to make implementation easier... */
struct identifier_type_t
{
    struct base_type_t base;
    char *identifier;
};

/* not really a type, but is here
to make implementation easier... */
struct reference_type_t
{
    struct base_type_t base;
    struct base_type_t *type;
};

/* not really a type, but is here
to make implementation easier... */
struct typedef_type_t
{
    struct base_type_t base;
    struct base_type_t *type;
};

#endif // TYPE_H






