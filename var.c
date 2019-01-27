#include "var.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static struct var_t *vars = NULL;
static struct var_t *last_var = NULL;


void create_var(char *id, int type)
{
    struct var_t *var;

    var = get_var(id);

    if(!var)
    {
        var = calloc(sizeof(struct var_t), 1);

        strcpy(var->id, id);
        var->type = type;

        if(!vars)
        {
            vars = var;
        }
        else
        {
            last_var->next = var;
        }

        last_var = var;
    }
}

void assign_var(char *id, struct var_value_t value)
{
    struct var_t *var;

    var = get_var(id);

    if(var)
    {
        if(value.var_type == var->type)
        {
            var->value = value;
        }
    }
}

struct var_t *get_var(char *id)
{
    struct var_t *var;

    var = vars;

    while(var)
    {
        if(!strcmp(var->id, id))
        {
            break;
        }
        var = var->next;
    }

    return var;
}

void clear_vars()
{
    while(vars)
    {
        last_var = vars->next;
        free(vars);
        vars = last_var;
    }
}

struct var_value_t value_from_str(char *str)
{
    struct var_value_t value;

    value.var_type = VAR_TYPE_INT;
    value.ivalue = atoi(str);
}










