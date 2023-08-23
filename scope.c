#include "scope.h"

struct scope_t *push_scope(struct parser_t *parser)
{
    // struct scope_t *scope;

    struct scope_t *scope = calloc(sizeof(struct scope_t), 1);
    scope->active = 1;
    if(parser->current_scope != NULL)
    {
        if(parser->current_scope->children == NULL)
        {
            parser->current_scope->children = scope;
        }
        else
        {
            parser->current_scope->last_child->next = scope;
        }

        parser->current_scope->last_child = scope;
    }
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
        parser->current_scope = scope->parent;

        // if(!scope->objects)
        // {
        //     /* only get rid of a scope when leaving it if no
        //     object lives in it...*/
        //     free(scope);
        // }
    }

    return parser->current_scope;
}

