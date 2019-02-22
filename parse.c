#include "parse.h"
#include "type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>




int is_type_specifier(struct token_t *token)
{
    if(token->token_type == TOKEN_KEYWORD)
    {
        switch(token->token_name)
        {
            case TOKEN_KEYWORD_INT:
            case TOKEN_KEYWORD_SHORT:
            case TOKEN_KEYWORD_LONG:
            case TOKEN_KEYWORD_CHAR:
            case TOKEN_KEYWORD_FLOAT:
            case TOKEN_KEYWORD_DOUBLE:
            case TOKEN_KEYWORD_SIGNED:
            case TOKEN_KEYWORD_UNSIGNED:
            case TOKEN_KEYWORD_VOID:
            case TOKEN_KEYWORD_STRUCT:
            case TOKEN_KEYWORD_UNION:
                return 1;
        }
    }

    return 0;
}

int is_type_qualifier(struct token_t *token)
{
    if(token->token_type == TOKEN_KEYWORD)
    {
        switch(token->token_name)
        {
            case TOKEN_KEYWORD_CONST:
            case TOKEN_KEYWORD_RESTRICT:
            case TOKEN_KEYWORD_VOLATILE:
                return 1;
        }
    }

    return 0;
}

int is_storage_class_specifier(struct token_t *token)
{
    if(token->token_type == TOKEN_KEYWORD)
    {
        switch(token->token_name)
        {
            case TOKEN_KEYWORD_TYPEDEF:
            case TOKEN_KEYWORD_EXTERN:
            case TOKEN_KEYWORD_STATIC:
            case TOKEN_KEYWORD_AUTO:
            case TOKEN_KEYWORD_REGISTER:
                return 1;
        }
    }

    return 0;
}

int is_typedef_name(struct token_t *token)
{
    return 0;
}

int type_from_token(struct token_t *token)
{
    switch(token->token_name)
    {
        case TOKEN_KEYWORD_INT:
            return TYPE_INT;

        case TOKEN_KEYWORD_VOID:
            return TYPE_VOID;

        case TOKEN_KEYWORD_SHORT:
            return TYPE_SHORT;

        case TOKEN_KEYWORD_CHAR:
            return TYPE_CHAR;

        case TOKEN_KEYWORD_FLOAT:
            return TYPE_FLOAT;

        case TOKEN_KEYWORD_DOUBLE:
            return TYPE_DOUBLE;

        case TOKEN_KEYWORD_STRUCT:
            return TYPE_STRUCT;

        case TOKEN_KEYWORD_UNION:
            return TYPE_UNION;

        default:
            return TYPE_UNKNOWN;
    }
}

char *type_string(struct base_type_t *type)
{
    switch(type->type)
    {
        case TYPE_INT:
            return "int";
        break;

        case TYPE_SHORT:
            return "short";
        break;

        case TYPE_CHAR:
            return "char";
        break;

        case TYPE_FLOAT:
            return "float";
        break;

        case TYPE_DOUBLE:
            return "double";
        break;

        case TYPE_LONG:
            return "long";
        break;

        case TYPE_UNSIGNED:
            return "unsigned";
        break;

        case TYPE_SIGNED:
            return "signed";
        break;

        case TYPE_VOID:
            return "void";
        break;

        case TYPE_STRUCT:
            return "struct";
        break;

        case TYPE_UNION:
            return "union";
        break;

        case TYPE_POINTER:
            return "pointer";
        break;

        case TYPE_FUNCTION:
            return "function";
        break;

        case TYPE_ARRAY:
            return "array";
        break;

        case TYPE_IDENTIFIER:
            return "identifier";
        break;

        case TYPE_UNKNOWN:
        default:
            return "unknown";
        break;
    }

}

void advance_token(struct parser_t *parser)
{
    parser->prev_token = parser->current_token;
    parser->current_token = parser->current_token->next;
}

struct scope_t *push_scope(struct parser_t *parser)
{
    struct scope_t *scope;

    scope = calloc(sizeof(struct scope_t ), 1);

    scope->parent = parser->current_scope;
    parser->current_scope = scope;

    parser->scope_stack_top++;
    parser->scope_stack[parser->scope_stack_top] = scope;


    return scope;
}

struct scope_t *pop_scope(struct parser_t *parser, int delete_scope)
{
    struct scope_t *scope = NULL;

    if(parser->scope_stack_top > 0)
    {
        scope = parser->current_scope;

        if(delete_scope)
        {
            free(scope);
        }

        parser->scope_stack_top--;

        parser->current_scope = parser->scope_stack[parser->scope_stack_top];
    }

    return parser->current_scope;
}

struct object_t *create_object(struct parser_t *parser, struct base_type_t *type)
{
    struct object_t *object;
    struct identifier_type_t *object_type;

    if(type->type == TYPE_REFERENCE)
    {
        type = ((struct reference_type_t *)type)->type;
    }

    assert(type->type == TYPE_IDENTIFIER);

    object_type = (struct identifier_type_t *)type;

    object = calloc(sizeof(struct object_t), 1);

    object->next = parser->objects;
    parser->objects = object;

    object->type = object_type->base.next;
    object->scope = parser->current_scope;
    object->id = strdup(object_type->identifier);

    return object;
}

int is_object_in_scope(struct parser_t *parser, struct object_t *object)
{
    struct scope_t *scope;

    scope = parser->current_scope;

    while(scope)
    {
        /* test whether we can get to the object's scope
        from the current scope or not. If yes, it's in scope... */
        if(scope == object->scope)
        {
            break;
        }

        scope = scope->parent;
    }

    /* null here means the object is out of scope... */
    return (int)scope;
}


void stash_aggregate_type(struct parser_t *parser, struct aggretage_type_t *type)
{
    struct aggretage_type_t *copy;

    copy = calloc(sizeof(struct aggretage_type_t), 1);

    copy->fields = type->fields;
    copy->name = type->name;

    copy->base.next = (struct base_type_t *)parser->aggregate_types;
    parser->aggregate_types = copy;
}


struct base_type_t *stash_typedef_type(struct parser_t *parser, struct base_type_t *type)
{

}

struct base_type_t *get_aggregate_type(struct parser_t *parser, char *name)
{
    struct aggretage_type_t *type;

    type = parser->aggregate_types;

    while(type)
    {
        if(!strcmp(name, type->name))
        {
            break;
        }

        type = (struct aggretage_type_t *)type->base.next;
    }

    return (struct base_type_t *)type;
}

/*void push_type(struct parser_t *parser, struct base_type_t *type)
{
    parser->decl_stack[parser->decl_stack_depth] = type;
    parser->decl_stack_depth++;
}*/

#define SCOPE_MAX_DEPTH 8192

void parse_tokens(struct token_t *tokens)
{
    struct token_t *token;
    struct parser_t parser;

    memset(&parser, 0, sizeof(struct parser_t));

    parser.tokens = tokens;
    parser.current_token = tokens;

    parser.scope_stack_top = -1;
    parser.scope_stack = calloc(sizeof(struct scope_t *), SCOPE_MAX_DEPTH);

    /* global (file) scope... */
    push_scope(&parser);

    while(parser.current_token->token_type != TOKEN_EOF)
    {
        if(is_type_specifier(parser.current_token))
        {
            translate_type((struct reference_type_t *)parse_declaration(&parser), 0);
        }
        else
        {
            advance_token(&parser);
        }
    }
}


struct base_type_t *parse_declaration(struct parser_t *parser)
{
    struct base_type_t *type = NULL;
    struct base_type_t *last_type = NULL;

    struct base_type_t *specifiers_qualifiers = NULL;
    struct base_type_t *last_specifier_qualifier = NULL;

    struct base_type_t *new_type = NULL;
    struct base_type_t *current_type = NULL;
    struct base_type_t *prev_type = NULL;
    struct base_type_t *declarator = NULL;
    struct base_type_t *temp_type = NULL;

    struct aggretage_type_t *aggretage_type;

    struct token_t *next_token;

    struct reference_type_t *type_reference = NULL;

    //if(is_type_specifier(parser->current_token))
    //{
    /* this is a declaration, which means
    we're expecting a bunch of type qualifiers,
    a few type specifiers, which are compatible
    with each other, and then a declarator.

    A declarator is defined as:
    pointer (optional) + direct-declarator



    A direct-declarator is defined as:
    identifier, a name;
    ( declarator ), a whole declarator inside parenthesis;
    direct-declarator + [type-qualifier-list (optional) assignment-expression(optional)];
    direct-declarator + [static type-qualifier-list (optional) assignment-expression];
    direct-declarator + [type-qualifier-list static assignment-expression];
    direct-declarator + [type-qualifier-list (optional) *];
    direct-declarator + (parameter-type-list);
    direct-declarator + (identifier-list (optional));


    This means that a declarator can be
    formed by several other declarators
    before we get to an actual identifier.

    This means that things like

    void *(*(*(*(*(*identifier)))))

    are valid, given that they can be rewritten as
    pointer + direct-declarator.

    For instance,

    type-qualifier declarator,
    type-qualifier pointer direct-declarator,
    type-qualifier pointer ( declarator ),
    type-qualifier pointer ( pointer direct-declarator )
    type-qualifier pointer ( pointer ( declarator ) )
    type-qualifier pointer ( pointer ( pointer direct-declarator ) )
    type-qualifier pointer ( pointer ( pointer ( declarator ) ) )
    type-qualifier pointer ( pointer ( pointer ( pointer direct-declarator ) ) )
    type-qualifier pointer ( pointer ( pointer ( pointer ( declarator ) ) ) )
    type-qualifier pointer ( pointer ( pointer ( pointer ( pointer direct-declarator ) ) ) )
    type-qualifier pointer ( pointer ( pointer ( pointer ( pointer ( declarator ) ) ) ) )
    type-qualifier pointer ( pointer ( pointer ( pointer ( pointer ( pointer direct-declarator ) ) ) ) )
    type-qualifier pointer ( pointer ( pointer ( pointer ( pointer ( pointer identifier ) ) ) ) )



    or void *(*func(int, int))(int, int, int)

    which can be decomposed into

    type-qualifier declarator,
    type-qualifier pointer direct-declarator,
    type-qualifier pointer direct-declarator(parameter-type-list),
    type-qualifier pointer ( declarator )(parameter-type-list),
    type-qualifier pointer ( pointer direct-declarator )(parameter-type-list),
    type-qualifier pointer ( pointer direct-declarator(parameter-type-list) )(parameter-type-list)




    The behavior we need to keep in mind is that a declarator can
    be defined recursively as a declarator.
    */

    do
    {
        if(parser->param_list_level)
        {
            specifiers_qualifiers = NULL;
        }

        if(is_type_specifier(parser->current_token) || is_type_qualifier(parser->current_token))
        {
            while(is_type_specifier(parser->current_token) || is_type_qualifier(parser->current_token))
            {
                if(parser->current_token->token_name == TOKEN_KEYWORD_STRUCT)
                {
                    next_token = parser->current_token->next;
                    new_type = NULL;

                    if(next_token->token_type == TOKEN_IDENTIFIER)
                    {
                        new_type = get_aggregate_type(parser, next_token->text);
                        next_token = next_token->next;
                    }

                    if(next_token->token_type == TOKEN_PUNCTUATOR &&
                       next_token->token_name == TOKEN_PUNCTUATOR_OBRACE)
                    {
                        if(new_type)
                        {
                            /* error: redefinition of aggregate type... */
                        }

                        /* we're declaring a struct here... */
                        new_type = parse_aggregate_definition(parser);
                    }
                }
                else
                {
                    new_type = calloc(sizeof(struct base_type_t), 1);
                    new_type->type = type_from_token(parser->current_token);
                    advance_token(parser);
                }

                if(!specifiers_qualifiers)
                {
                    specifiers_qualifiers = new_type;
                }
                else
                {
                    last_specifier_qualifier->next = new_type;
                }

                last_specifier_qualifier = new_type;
            }
        }
        else if(parser->current_token->token_type == TOKEN_IDENTIFIER)
        {
            if(is_typedef_name(parser->current_token))
            {

            }
            else
            {
                /* error: unknown type... */
            }
        }

        current_type = specifiers_qualifiers;

        if(current_type)
        {
            /* declarator: identifier, function, array, pointer, etc... */
            declarator = parse_declarator(parser);

            if(declarator)
            {
                temp_type = declarator;

                while(temp_type->next)
                {
                    temp_type = temp_type->next;
                }

                temp_type->next = specifiers_qualifiers;

                current_type = declarator;
            }

            type_reference = calloc(sizeof(struct reference_type_t), 1);
            type_reference->base.next = NULL;
            type_reference->base.type = TYPE_REFERENCE;

            type_reference->type = current_type;

            if(!type)
            {
                type = (struct base_type_t *)type_reference;
            }
            else
            {
                last_type->next = (struct base_type_t *)type_reference;
            }

            last_type = (struct base_type_t *)type_reference;
        }

        if(parser->current_token->token_type != TOKEN_PUNCTUATOR ||
           parser->current_token->token_name != TOKEN_PUNCTUATOR_COMMA)
        {
            break;
        }

        if(!current_type)
        {
            /* error: expecting expression before token ',' */
        }

        advance_token(parser);

    }while(1);
    //}

    return type;
}


struct base_type_t *parse_declarator(struct parser_t *parser)
{
    struct base_type_t *type = NULL;
    struct base_type_t *ret_type = NULL;
    struct base_type_t *temp = NULL;
    struct base_type_t *new_type = NULL;

    struct function_type_t *function_type = NULL;
    struct array_type_t *array_type = NULL;
    struct base_type_t *pointer_type = NULL;
    struct identifier_type_t *identifier = NULL;
    struct reference_type_t *params_or_fields = NULL;


    if(is_type_specifier(parser->current_token))
    {
        /* welp, this is an error... */
    }


    /* optional pointer before the direct-declarator... */
    if(parser->current_token->token_type == TOKEN_PUNCTUATOR)
    {
        if(parser->current_token->token_name == TOKEN_PUNCTUATOR_ASTERISC)
        {
            while(parser->current_token->token_type == TOKEN_PUNCTUATOR &&
                  parser->current_token->token_name == TOKEN_PUNCTUATOR_ASTERISC)
            {
                new_type = calloc(1, sizeof(struct base_type_t));
                new_type->type = TYPE_POINTER;

                new_type->next = pointer_type;
                pointer_type = new_type;

                advance_token(parser);
            }
        }
        else
        {
            /* everything else is wrong... */
        }
    }





    /* direct-declarator: can be an identifier, an
    declarator enclosed in parenthesis, or
    an direct-declarator... */
    switch(parser->current_token->token_type)
    {
        case TOKEN_IDENTIFIER:
            identifier = calloc(sizeof(struct identifier_type_t), 1);

            identifier->base.next = NULL;
            identifier->base.type = TYPE_IDENTIFIER;
            identifier->identifier = parser->current_token->text;

            advance_token(parser);
        break;

        case TOKEN_PUNCTUATOR:
            if(parser->current_token->token_name == TOKEN_PUNCTUATOR_OPARENTHESIS)
            {
                advance_token(parser);
                ret_type = parse_declarator(parser);

                if(parser->current_token->token_type == TOKEN_PUNCTUATOR &&
                   parser->current_token->token_name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                {
                    advance_token(parser);
                }
                else
                {
                    /* everything else is wrong... */
                }
            }
            else
            {
                /* everything else is wrong... */
            }
        break;
    }





    if(parser->current_token->token_type == TOKEN_PUNCTUATOR)
    {
        /* this declarator may be a function or an array... */
        switch(parser->current_token->token_name)
        {
            case TOKEN_PUNCTUATOR_OPARENTHESIS:

                function_type = calloc(sizeof(struct function_type_t), 1);

                function_type->base.next = NULL;
                function_type->base.type = TYPE_FUNCTION;

                /* ( */
                advance_token(parser);

                /* this is the parameter list of a function, which means we'll be
                possibly declaring other identifiers, so we call the declaration
                parsing routine here...*/

                params_or_fields = NULL;

                if(parser->current_token->token_type != TOKEN_PUNCTUATOR ||
                   parser->current_token->token_name != TOKEN_PUNCTUATOR_CPARENTHESIS)
                {
                    parser->param_list_level++;
                    params_or_fields = parse_declaration(parser);
                    parser->param_list_level--;
                }

                if(!params_or_fields)
                {
                    /* empty params, implies void
                    param... */
                    params_or_fields = calloc(sizeof(struct reference_type_t), 1);
                    params_or_fields->type = calloc(sizeof(struct base_type_t), 1);
                    params_or_fields->type->type = TYPE_VOID;
                }


                function_type->params = params_or_fields;

                /* ) */
                advance_token(parser);

            break;

            case TOKEN_PUNCTUATOR_OBRACKET:
                array_type = calloc(sizeof(struct array_type_t), 1);
                advance_token(parser);
            break;

            default:
                /* anything else is wrong... */
            break;
        }
    }
    else
    {
        /* anything else is wrong... */
    }




    if(function_type)
    {
        type = (struct base_type_t *) function_type;

        if(pointer_type)
        {
            /* function that returns pointer to... */
            function_type->base.next = pointer_type;
        }
    }
    else
    {
        /* pointer to... */
        type = pointer_type;
    }

    if(identifier)
    {
        /* identifier is a pointer to/ is a function... */
        identifier->base.next = type;
        type = (struct base_type_t *)identifier;
    }

    if(ret_type)
    {
        temp = ret_type;

        while(temp->next)
        {
            temp = temp->next;
        }

        switch(temp->type)
        {
            case TYPE_FUNCTION:

                if(function_type)
                {
                    /* error: function returning function type... */
                }

                if(array_type)
                {
                    /* error: function returning array type... */
                }

                if(identifier)
                {
                    /* error: unexpected identifier... */
                }

                function_type = (struct function_type_t *)temp;

            break;
        }

        temp->next = type;

        type = ret_type;
    }

    return type;
}


struct base_type_t *parse_aggregate_definition(struct parser_t *parser)
{
    struct aggretage_type_t *type;

    struct reference_type_t *field = NULL;
    struct reference_type_t *fields = NULL;
    struct reference_type_t *last_field = NULL;

    type = calloc(sizeof(struct aggretage_type_t), 1);

    if(parser->current_token->token_type == TOKEN_KEYWORD)
    {
        switch(parser->current_token->token_name)
        {
            case TOKEN_KEYWORD_STRUCT:
                type->base.type = TYPE_STRUCT;
            break;

            case TOKEN_KEYWORD_UNION:
                type->base.type = TYPE_UNION;
            break;
        }
    }
    else
    {
        /* everything else is wrong... */
    }

    advance_token(parser);

    if(parser->current_token->token_type == TOKEN_IDENTIFIER)
    {
        type->name = strdup(parser->current_token->text);
        advance_token(parser);
    }


    if(parser->current_token->token_type == TOKEN_PUNCTUATOR &&
       parser->current_token->token_name == TOKEN_PUNCTUATOR_OBRACE)
    {
        advance_token(parser);

        while(1)
        {
            field = parse_declaration(parser);

            if(parser->current_token->token_type == TOKEN_PUNCTUATOR &&
               parser->current_token->token_name == TOKEN_PUNCTUATOR_SEMICOLON)
            {
                advance_token(parser);
            }

            if(!fields)
            {
                fields = field;
            }
            else
            {
                last_field->base.next = field;
            }

            last_field = field;

            if(parser->current_token->token_type == TOKEN_PUNCTUATOR &&
               parser->current_token->token_name == TOKEN_PUNCTUATOR_CBRACE)
            {
                advance_token(parser);
                break;
            }

            //advance_token(parser);
        }

        type->fields = fields;
    }

    if(type->name)
    {
        /* this aggregate type has a tag, so we stash it... */
        stash_aggregate_type(parser, type);
    }

    return (struct base_type_t *)type;
}







void translate_type(struct reference_type_t *type, int single_reference)
{
    struct identifier_type_t *identifier;
    struct function_type_t *function;
    struct reference_type_t *param_or_field;
    struct base_type_t *base_type;
    struct aggretage_type_t *aggretage_type;

    while(type)
    {
        base_type = type->type;

        while(base_type)
        {
            switch(base_type->type)
            {
                case TYPE_IDENTIFIER:
                    identifier = (struct identifier_type_t *)base_type;
                    printf("[%s] is ", identifier->identifier);
                break;

                case TYPE_POINTER:
                    printf("a pointer to ");
                break;

                case TYPE_FUNCTION:
                    function = (struct function_type_t *)base_type;

                    printf("a function (");

                    param_or_field = function->params;

                    while(param_or_field)
                    {
                        translate_type(param_or_field, 1);

                        param_or_field = (struct reference_type_t *)param_or_field->base.next;

                        if(param_or_field)
                        {
                            printf(", ");
                        }
                    }

                    printf("), returning ");
                break;

                case TYPE_STRUCT:
                    aggretage_type = (struct aggretage_type_t *)base_type;

                    if(aggretage_type->name)
                    {
                        printf("a struct [%s] {", aggretage_type->name);
                    }
                    else
                    {
                        printf("an anonymous struct {");
                    }

                    param_or_field = aggretage_type->fields;

                    while(param_or_field)
                    {
                        translate_type(param_or_field, 1);

                        param_or_field = param_or_field->base.next;

                        if(param_or_field)
                        {
                            printf(", ");
                        }
                    }

                    printf("}, ");


                break;

                case TYPE_INT:
                    printf("an int");
                break;

                case TYPE_FLOAT:
                    printf("a float");
                break;

                case TYPE_DOUBLE:
                    printf("a double");
                break;

                case TYPE_VOID:
                    printf("void");
                break;

                case TYPE_SHORT:
                    printf("a short");
                break;

                case TYPE_CHAR:
                    printf("a char");
                break;

                default:
                    return;
                break;
            }

            base_type = base_type->next;
        }

        type = single_reference ? NULL : (struct reference_type_t *)type->base.next;
    }

    //printf("\n");
}











