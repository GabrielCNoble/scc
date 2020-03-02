#include "scope.h"

struct scope_t *push_scope(struct parser_t *parser)
{
    struct scope_t *scope;

    scope = calloc(sizeof(struct scope_t), 1);
    scope->active = 1;
    scope->parent = parser->current_scope;
    parser->current_scope = scope;


    return scope;
}

struct scope_t *pop_scope(struct parser_t *parser)
{
    struct scope_t *scope = NULL;

    if(parser->current_scope)
    {
        scope = parser->current_scope;
        scope->active = 0;

        if(!scope->objects)
        {
            /* only get rid of a scope when leaving it if no
            object lives in it...*/
            free(scope);
        }

        parser->current_scope = scope->parent;
    }

    return parser->current_scope;
}

