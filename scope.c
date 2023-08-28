#include "scope.h"

struct scope_t *push_scope(struct parser_t *parser)
{
    // struct scope_t *scope;

    struct scope_t *scope = pool_AddElement(&parser->scopes, NULL);
    scope->active = 1;
    if(parser->cur_scope != NULL)
    {
        if(parser->cur_scope->children == NULL)
        {
            parser->cur_scope->children = scope;
        }
        else
        {
            parser->cur_scope->last_child->next = scope;
        }

        parser->cur_scope->last_child = scope;
    }
    scope->parent = parser->cur_scope;
    parser->cur_scope = scope;

    return scope;
}

struct scope_t *pop_scope(struct parser_t *parser)
{
    struct scope_t *scope = NULL;

    if(parser->cur_scope)
    {
        scope = parser->cur_scope;
        scope->active = 0;
        parser->cur_scope = scope->parent;
    }

    return parser->cur_scope;
}

