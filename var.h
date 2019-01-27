#ifndef VAR_H
#define VAR_H



enum VAR_TYPE
{
    VAR_TYPE_INT = 0,
    VAR_TYPE_FLOAT,
};


struct var_value_t
{
    int var_type;
    union
    {
        int ivalue;
        float fvalue;
    };
};


struct var_t
{
    struct var_t *next;
    char id[64];
    int type;
    struct var_value_t value;
};



void create_var(char *id, int type);

void assign_var(char *id, struct var_value_t value);

struct var_t *get_var(char *id);

void clear_vars();

struct var_value_t value_from_str(char *str);



#endif // VAR_H





