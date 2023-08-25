#include "obj.h"

struct object_t *create_object(struct parser_t *parser, struct declarator_t *declarator)
{
    struct object_t *object = NULL;

    if(declarator->identifier != NULL)
    {
        object = get_object(parser, declarator->identifier);

        if(object == NULL)
        {
            object = calloc(1, sizeof(struct object_t));
            object->declarator = declarator;
            object->scope = parser->cur_scope;

            if(parser->cur_scope->objects == NULL)
            {
                parser->cur_scope->objects = object;                
            }
            else
            {
                parser->cur_scope->last_object->next = object;
            }

            parser->cur_scope->last_object = object;
        }

    }

    return object;
}

struct object_t *get_object(struct parser_t *parser, char *identifier)
{
    struct object_t *object = NULL;
    struct scope_t *scope;

    /* the search will start at the current scope,
    and move upwards towards the scope's parents.
    This guarantees that objects in the outer
    scopes will be shadowed by objects in inner
    scopes that have the same identifier */
    scope = parser->cur_scope;
    
    while(scope)
    {
        object = scope->objects;

        while(object)
        {
            if(!strcmp(object->declarator->identifier, identifier))
            {
                return object;
            }

            object = object->next;
        }

        scope = scope->parent;
    }

    return NULL;
}

int is_object_in_scope(struct parser_t *parser, struct object_t *object)
{
    return object->scope->active;
}
