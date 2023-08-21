#include "obj.h"

struct object_t *create_object(struct parser_t *parser, struct base_type_t *type)
{
    struct object_t *object;
    struct identifier_type_t *object_type;

    if(type->type == TYPE_LINK)
    {
        type = ((struct link_type_t *)type)->type;
    }

    object_type = (struct identifier_type_t *)type;

    object = calloc(sizeof(struct object_t), 1);
    object->type = object_type->base.next;
    object->scope = parser->current_scope;
    object->id = strdup(object_type->identifier);

    if(!parser->current_scope->objects)
    {
        parser->current_scope->objects = object;
    }
    else
    {
        parser->current_scope->last_object->next = object;
    }

    parser->current_scope->last_object = object;

    return object;
}

struct object_t *get_object(struct parser_t *parser, char *id)
{
    struct object_t *object;
    struct scope_t *scope;

    /* the search will start at the current scope,
    and move upwards towards the scope's parents.
    This guarantees that objects in the outer
    scopes will be shadowed by objects in inner
    scopes that have the same identifier */
    scope = parser->current_scope;
    while(scope)
    {
        object = scope->objects;

        while(object)
        {
            if(!strcmp(object->id, id))
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
