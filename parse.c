#include "parse.h"
#include "type.h"
#include "exp.h"

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
            case TOKEN_KEYWORD_ENUM:
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

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

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

struct scope_t *pop_scope(struct parser_t *parser)
{
    struct scope_t *scope = NULL;

    if(parser->scope_stack_top > 0)
    {
        scope = parser->current_scope;

        if(!scope->objects)
        {
            /* only get rid of a scope when leaving it if no
            object lives in it...*/
            free(scope);
        }

        parser->scope_stack_top--;

        parser->current_scope = parser->scope_stack[parser->scope_stack_top];
    }

    return parser->current_scope;
}

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

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
//    struct scope_t *scope;
    int i;

    for(i = parser->scope_stack_top; i >= 0; i--)
    {
        /* start by testing the innermost scopes, as
        objects can have the same identifier if they get
        declared in different scopes, and objects in
        inner scopes shadow objects in outer scopes...*/
        object = parser->scope_stack[i]->objects;

        while(object)
        {
            if(!strcmp(id, object->id))
            {
                return object;
            }

            object = object->next;
        }
    }

    return NULL;
}

int is_object_in_scope(struct parser_t *parser, struct object_t *object)
{
    int i;

    for(i = parser->scope_stack_top; i >= 0; i--)
    {
        if(parser->scope_stack[i] == object->scope)
        {
            /* if the scope in which the object exists
            is in the scope stack, the object will be in
            scope... */
            break;
        }
    }

    /* null here means the object is out of scope... */
    return i >= 0;
}

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

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

struct base_type_t *copy_type(struct base_type_t *type)
{
    struct base_type_t *new_type = NULL;
    struct base_type_t *first_type = NULL;
    struct base_type_t *last_type = NULL;

    struct reference_type_t *reference;
    struct reference_type_t *args;
    struct reference_type_t *last_arg;
    struct function_type_t *function;
    struct aggretage_type_t *aggretage;

    struct aggretage_type_t *aggretage_type;
    struct function_type_t *function_type;
    struct reference_type_t *reference_type;

    int size;

    while(type)
    {
        switch(type->type)
        {
        case TYPE_INT:
            case TYPE_SHORT:
            case TYPE_CHAR:
            case TYPE_FLOAT:
            case TYPE_DOUBLE:
            case TYPE_LONG:
            case TYPE_UNSIGNED:
            case TYPE_CONST:
            case TYPE_VOLATILE:
            case TYPE_RESTRICT:
            case TYPE_EXTERN:
            case TYPE_SIGNED:
            case TYPE_VOID:
            case TYPE_POINTER:
                size = sizeof(struct base_type_t);
            break;

            case TYPE_STRUCT:
            case TYPE_UNION:
                size = sizeof(struct aggretage_type_t);
            break;

            case TYPE_FUNCTION:
                size = sizeof(struct function_type_t);
            break;

            case TYPE_ARRAY:
                size = sizeof(struct array_type_t);
            break;

            case TYPE_TYPEDEF:
                size = sizeof(struct typedef_type_t);
            break;

            case TYPE_IDENTIFIER:
                size = sizeof(struct identifier_type_t);
            break;

            case TYPE_REFERENCE:
                size = sizeof(struct reference_type_t);
            break;
        }

        new_type = calloc(size, 1);
        memcpy(new_type, type, size);
        new_type->next = NULL;

        if(new_type->type == TYPE_FUNCTION)
        {
            function_type = (struct function_type_t *)new_type;
            function = (struct function_type_t *)type;
            function_type->args = copy_type(function->args);
        }
        else if(new_type->type == TYPE_REFERENCE)
        {
            reference_type = (struct reference_type_t *)new_type;
            reference = (struct reference_type_t *)type;
            reference_type->type = copy_type(reference->type);
        }

        if(!first_type)
        {
            first_type = new_type;
        }
        else
        {
            last_type->next = new_type;
        }

        last_type = new_type;

        type = type->next;
    }

    return first_type;
}

int validate_type(struct base_type_t *type)
{
    int has_short = 0;
    int has_int = 0;
    int id_found = 0;
    int has_const = 0;
    int has_volatile = 0;
    int has_restrict = 0;

    int cur_decl_token;

//    while(type)
//    {
//        switch(type->type)
//        {
//            case TYPE_INT:
//            case TYPE_SHORT:
//            case TYPE_FLOAT:
//            case TYPE_DOUBLE:
//            case TYPE_CHAR:
//                switch(type->type)
//                {
//                    case TYPE_INT:
//                        has_int =
//                    break;
//                }
//                has_const = 0;
//                has_volatile = 0;
//                cur_decl_token = DECL_TOKEN_SPECIFIER;
//            break;
//
//            case TYPE_CONST:
//            case TYPE_VOLATILE:
//            case TYPE_RESTRICT:
//                switch(type->type)
//                {
//                    case TYPE_CONST:
//                        has_const = 1;
//                    break;
//
//                    case TYPE_VOLATILE:
//                        has_volatile = 1;
//                    break;
//
//                    case TYPE_RESTRICT:
//                        has_restrict = 1;
//                    break;
//                }
//                cur_decl_token = DECL_TOKEN_QUALIFIER;
//            break;
//        }
//
//        type = type->next;
//    }
}

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/



#define SCOPE_MAX_DEPTH 8192

void parse_tokens(struct token_t *tokens)
{
    struct parser_t parser;

    struct reference_type_t *types;

    memset(&parser, 0, sizeof(struct parser_t));

    parser.tokens = tokens;
    parser.current_token = tokens;

    parser.scope_stack_top = -1;
    parser.scope_stack = calloc(sizeof(struct scope_t *), SCOPE_MAX_DEPTH);

    parser.global_bytecode.buffer_size = 1024;
    parser.global_bytecode.bytecode = calloc(parser.global_bytecode.buffer_size, 1);

    parser.reg_index = 0;

    parser.declaration_depth = -1;
    /* global (file) scope... */
    push_scope(&parser);

    parse_declaration(&parser, 0);

    //while(parser.current_token->token_type != TOKEN_EOF)
    {
//        if(is_type_specifier(parser.current_token))
//        {
//            types = (struct reference_type_t *)parse_declaration(&parser);
//        }
//        else
//        {
//            advance_token(&parser);
//        }

        //exp_16(&parser);
    }

    pop_scope(&parser);
}

struct base_type_t *parse_declaration(struct parser_t *parser, int is_in_arg_list)
{
    struct base_type_t *type = NULL;
    struct base_type_t *last_type = NULL;

    struct base_type_t *specifiers_qualifiers = NULL;
    struct base_type_t *last_specifier_qualifier = NULL;

    struct base_type_t *new_type = NULL;
    struct base_type_t *current_type = NULL;
//    struct base_type_t *prev_type = NULL;
    struct base_type_t *declarator = NULL;
    struct base_type_t *temp_type = NULL;

    struct reference_type_t *params;
    struct reference_type_t *next_param;

    struct function_type_t *function_type;

    struct object_t *object;
    int old_function_style;

//    struct aggretage_type_t *aggretage_type;
    struct identifier_type_t *identifier;

    struct identifier_type_t *identifiers;
    struct identifier_type_t *last_identifier;

    struct token_t *next_token;

    struct reference_type_t *type_reference = NULL;
    struct reference_type_t *prev_type_reference;

    parser->declaration_depth++;


    //if(is_type_specifier(parser->current_token))
    //{
    /* this is a declaration, which means
    we're expecting a bunch of type qualifiers,
    a few type specifiers, which are compatible
    with each other, and then a declarator.


    this is a declaration, which is of the
    form:

        declaration-specifiers init-declarator-list;

    where declaration-specifiers can be:

        storage-class-specifier (static, extern, auto, register) declaration-specifier(opt);
        type-specifier(int, float, char, union, struct, enum, typedef) declaration-specifier(opt);
        type-qualifier(const, volatile, restrict) declaration-specifier(opt);
        function-specifier(inline) declaration-specifier(opt);

    where init-declarator-list can be:

        init-declarator;
        init-declarator-list, init-declarator;

    and init-declarator can be:

        declarator;
        declarator = initializer;

    */

    if(is_type_specifier(parser->current_token) || is_type_qualifier(parser->current_token))
    {
        while(is_type_specifier(parser->current_token) || is_type_qualifier(parser->current_token))
        {
            if(parser->current_token->token_name == TOKEN_KEYWORD_STRUCT ||
               parser->current_token->token_name == TOKEN_KEYWORD_UNION)
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

                    /* we're declaring a struct/union here... */
                    new_type = parse_aggregate_declaration(parser);
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

    while(1)
    {
        /* declarator: identifier, function, array, pointer, etc... */
        declarator = parse_declarator(parser);

        if(declarator)
        {
            /* got a declarator, which possibly have a lot of stuff linked,
            so find the end of the declarator and append the declaration-specifier
            we found... */
            temp_type = declarator;

            while(temp_type->next)
            {
                temp_type = temp_type->next;
            }

            temp_type->next = specifiers_qualifiers;

            current_type = declarator;
        }

        /* having an declarator here is not obligatory, given that this
        code will be used to find out the type when the type-cast operator
        gets used... */

        if(!parser->declaration_depth)
        {
            /* since identifiers gets treated as types for the sake
            of implementation they'll alway will be the first element
            in the list of a declaration (if present) */
            if(current_type->type == TYPE_IDENTIFIER)
            {
                identifier = (struct identifier_type_t *)current_type;
                temp_type = (struct base_type_t *)current_type->next;

                switch(temp_type->type)
                {
                    case TYPE_FUNCTION:
                        function_type = (struct function_type_t *)temp_type;
                        type_reference = function_type->args;

                        if(function_type->arg_count)
                        {
                            if(function_type->old_style)
                            {
                                /* the param types are being specified after
                                the declarator, so parse them... */
                            }
                        }

                        if(parser->current_token->token_type == TOKEN_PUNCTUATOR)
                        {
                            switch(parser->current_token->token_name)
                            {
                                case TOKEN_PUNCTUATOR_SEMICOLON:
                                    advance_token(parser);
                                break;

                                case TOKEN_PUNCTUATOR_OBRACE:
                                    /* function body... */

                                   // parse_compound_statement(parser, )
                                break;

                                default:
                                    /* everything else is wrong... */
                                break;
                            }
                        }
                        else
                        {
                            /* everything else is wrong... */
                        }

                    break;

                    default:
                        object = get_object(parser, identifier->identifier);

                        if(!object || (object->scope != parser->current_scope))
                        {
                            /* an object with the current name already exists,
                            but it's declared in a different scope, so creating
                            another object with the same name in this scope isn't
                            considered duplication... */
                            create_object(parser, current_type);
                        }
                        else
                        {
                            /* error: duplicate object... */
                        }
                    break;
                }

            }
        }

        if(!is_in_arg_list)
        {
            if(parser->current_token->token_type == TOKEN_PUNCTUATOR)
            {
                if(parser->current_token->token_name == TOKEN_PUNCTUATOR_ASSIGN)
                {
                    /* deal with assignment, call the expression routines... */
                }

                if(parser->current_token->token_name == TOKEN_PUNCTUATOR_SEMICOLON)
                {
                    advance_token(parser);
                    break;
                }
            }
            else
            {
                /* error: expecting '=', ',' or ';'... */
            }

            if(parser->current_token->token_name == TOKEN_PUNCTUATOR_COMMA)
            {
                specifiers_qualifiers = copy_type(specifiers_qualifiers);
            }
            else
            {
                /* error: some non-sense going on... */
            }
        }

        advance_token(parser);
    }


    parser->declaration_depth--;

    return current_type;
}

struct base_type_t *parse_declarator(struct parser_t *parser)
{
    struct base_type_t *type = NULL;
    struct base_type_t *ret_declarator = NULL;
    struct base_type_t *temp = NULL;
    struct base_type_t *new_type = NULL;

    struct function_type_t *function_type = NULL;
    struct array_type_t *array_type = NULL;
    struct base_type_t *pointer_type = NULL;
    struct identifier_type_t *identifier = NULL;
    struct reference_type_t *params_or_fields = NULL;
    struct reference_type_t *param_or_field = NULL;
    struct reference_type_t *last_param_or_field = NULL;

    /*
        A declarator is defined as:

            pointer (optional) + direct-declarator;


        A direct-declarator is defined as:

            identifier, a name;
            ( declarator ), a whole declarator inside parenthesis;
            direct-declarator + [type-qualifier-list (optional) assignment-expression(optional)];
            direct-declarator + [static type-qualifier-list (optional) assignment-expression];
            direct-declarator + [type-qualifier-list static assignment-expression];
            direct-declarator + [type-qualifier-list (optional) *];
            direct-declarator + (parameter-type-list);
            direct-declarator + (identifier-list (optional));


        where pointer is:

            * + type-qualifier-list (optional)
            * + type-qualifier-list (optional) + *


        where type-qualifier-list is:

            type-qualifier (which means this can span a new declaration);
            type-qualifier-list type-qualifier;


        where parameter-type-list is:

            parameter-list;
            parameter-list, ...


        where parameter-list is:

            parameter-declaration;
            parameter-list, parameter-declaration;


        where parameter-declaration is:

            declaration-specifiers (a new declaration) + declarator,
            declaration-specifiers (a new declaration) + abstract-declarator (?)


        where identifier-list is:

            identifier;
            identifier-list, identifier;



        This means that a declarator can be
        formed by several other declarators
        before we get to an actual identifier.

        This means that things like

        void *(*(*(*(*(*identifier)))))

        are valid, given that they can be rewritten as
        pointer + direct-declarator.

        For instance,

        type-specifier declarator,
        type-specifier pointer direct-declarator,
        type-specifier pointer ( declarator ),
        type-specifier pointer ( pointer direct-declarator )
        type-specifier pointer ( pointer ( declarator ) )
        type-specifier pointer ( pointer ( pointer direct-declarator ) )
        type-specifier pointer ( pointer ( pointer ( declarator ) ) )
        type-specifier pointer ( pointer ( pointer ( pointer direct-declarator ) ) )
        type-specifier pointer ( pointer ( pointer ( pointer ( declarator ) ) ) )
        type-specifier pointer ( pointer ( pointer ( pointer ( pointer direct-declarator ) ) ) )
        type-specifier pointer ( pointer ( pointer ( pointer ( pointer ( declarator ) ) ) ) )
        type-specifier pointer ( pointer ( pointer ( pointer ( pointer ( pointer direct-declarator ) ) ) ) )
        type-specifier pointer ( pointer ( pointer ( pointer ( pointer ( pointer identifier ) ) ) ) )



        or void *(*func(int, int))(int, int, int)

        which can be decomposed into

        type-specifier declarator,
        type-specifier pointer direct-declarator,
        type-specifier pointer direct-declarator(parameter-type-list),
        type-specifier pointer ( declarator )(parameter-type-list),
        type-specifier pointer ( pointer direct-declarator )(parameter-type-list),
        type-specifier pointer ( pointer direct-declarator(parameter-type-list) )(parameter-type-list)


        The behavior we need to keep in mind is that a declarator can
        be defined recursively as a declarator.
    */


//    if(is_type_specifier(parser->current_token))
//    {
//        /* welp, this is an error... */
//    }


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
                ret_declarator = parse_declarator(parser);

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
                last_param_or_field = NULL;

                /*if(parser->current_token->token_type != TOKEN_PUNCTUATOR ||
                   parser->current_token->token_name != TOKEN_PUNCTUATOR_CPARENTHESIS)
                {*/
                //parser->param_list_level++;

                function_type->args = 0;

                while(1)
                {
                    /* parser parameter-list... */
                    new_type = parse_declaration(parser, 1);

                    if(new_type)
                    {
                        function_type->arg_count++;

                        param_or_field = calloc(sizeof(struct reference_type_t), 1);
                        param_or_field->type = new_type;

                        if(!params_or_fields)
                        {
                            params_or_fields = param_or_field;
                        }
                        else
                        {
                            last_param_or_field->base.next = param_or_field;
                        }

                        last_param_or_field = param_or_field;
                    }

                    if(parser->current_token->token_type == TOKEN_PUNCTUATOR)
                    {
                        switch(parser->current_token->token_name)
                        {
                            case TOKEN_PUNCTUATOR_CPARENTHESIS:
                                /* end of the argument list... */
                                break;
                            break;

                            case TOKEN_PUNCTUATOR_COMMA:
                                /* , */
                                advance_token(parser);
                            break;

                            default:
                                /* error: unexpected token. Expecting ')' or ','... */
                            break;
                        }
                    }
                }

                //parser->param_list_level--;
                //}

                function_type->args = params_or_fields;

                /* ) */
                advance_token(parser);


//                if(!parser->param_list_level)
//                {
//                    /* if the param_list_level is zero,it means we're not dealing with declarators
//                    that are inside a param list. So, here we find out whether this function declaration
//                    is using the new or the old style... */
//
//                    if(function_type->param_count)
//                    {
//
//                        /* first param will tell us whether this is
//                        new or old style declaration... */
//                        param_or_field = function_type->params;
//
//                        temp = param_or_field->type;
//
//                        if(temp->type != TYPE_IDENTIFIER)
//                        {
//                            /* error: something something... */
//                        }
//
//                        temp = temp->next;
//
//                        if(!temp)
//                        {
//                            /* if this the old style declaration, the only thing
//                            we get in each parameter is its identifier... */
//
//                            function_type->old_style = 1;
//                        }
//
//                        param_or_field = (struct reference_type_t *)param_or_field->base.next;
//
//                        while(param_or_field)
//                        {
//                            temp = param_or_field->type;
//
//                            if(temp->type != TYPE_IDENTIFIER)
//                            {
//                                /* error: missing parameter name... */
//                            }
//
//                            temp = temp->next;
//
//                            if((temp && 1) == function_type->old_style)
//                            {
//                                /* error: mixing old style with prototype... */
//                            }
//
//                            param_or_field = (struct reference_type_t *)param_or_field->base.next;
//                        }
//                    }
//                }

            break;

            case TOKEN_PUNCTUATOR_OBRACKET:
                array_type = calloc(sizeof(struct array_type_t), 1);

                /* [... */
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
        /* identifier is a function/array of/pointer to/nothing... */
        identifier->base.next = type;
        type = (struct base_type_t *)identifier;
    }

    if(ret_declarator)
    {
        temp = ret_declarator;

        while(temp)
        {
            /* we skip until the end of the list, since we'll be
            appending what we found in this call to what was
            found in the recursive call.

            If this is a function, the last element of the list will
            be the function type, since its return type is kept in the
            next pointer, and up to this point we still don't know its
            return type (since it was found in the current call)... */
            temp = temp->next;
        }

        switch(temp->type)
        {
            case TYPE_FUNCTION:

                if(function_type)
                {
                    /* error: function returning function... */
                }

                if(array_type)
                {
                    /* error: function returning array... */
                }

                function_type = (struct function_type_t *)temp;

            break;
        }

        temp->next = type;

        type = ret_declarator;
    }

    return type;
}


struct base_type_t *parse_aggregate_declaration(struct parser_t *parser)
{
    struct aggretage_type_t *type;

    struct base_type_t *field_type = NULL;
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
            field_type = parse_declaration(parser, 0);

            field = calloc(sizeof(struct reference_type_t ), 1);

            field->type = field_type;
            field->base.type = TYPE_REFERENCE;
            field->base.next = NULL;

            if(!fields)
            {
                fields = field;
            }
            else
            {
                last_field->base.next = (struct base_type_t *)field;
            }

            last_field = field;

            if(parser->current_token->token_type == TOKEN_PUNCTUATOR &&
               parser->current_token->token_name == TOKEN_PUNCTUATOR_CBRACE)
            {
                advance_token(parser);
                break;
            }
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




void parse_statement(struct parser_t *parser)
{
    struct token_t *token;

    switch(parser->current_token->token_type)
    {
        case TOKEN_KEYWORD:
            switch(parser->current_token->token_name)
            {
                case TOKEN_KEYWORD_IF:
                    /* if statement... */

                    /* 'if' */
                    advance_token(parser);

                    if(parser->current_token->token_type == TOKEN_PUNCTUATOR ||
                       parser->current_token->token_name == TOKEN_PUNCTUATOR_OPARENTHESIS)
                    {

                        /* parse expression here... */

                        if(parser->current_token->token_type == TOKEN_PUNCTUATOR ||
                           parser->current_token->token_name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                        {
                            /* ')' */
                            advance_token(parser);
                        }
                        else
                        {
                            /* error: expecting token ')'... */
                        }
                    }
                    else
                    {
                        /* error: expecting token '('... */
                    }
                break;

                case TOKEN_KEYWORD_SWITCH:
                    /* switch statement... */

                    /* 'switch' */
                    advance_token(parser);

                    if(parser->current_token->token_type == TOKEN_PUNCTUATOR ||
                       parser->current_token->token_name == TOKEN_PUNCTUATOR_OBRACE)
                    {

                        /* parse compound statement here. The compound statement parsing
                        routine will already skip '{' and '}'... */
                    }
                    else
                    {
                        /* error: expecting token '{'... */
                    }

                break;

                case TOKEN_KEYWORD_FOR:
                case TOKEN_KEYWORD_WHILE:
                case TOKEN_KEYWORD_DO:

                    /* for/while/do... */
                    advance_token(parser);

                break;


                default:
                    /* everything else is wrong... */
                break;
            }
        break;

        case TOKEN_PUNCTUATOR:
            if(parser->current_token->token_name == TOKEN_PUNCTUATOR_OBRACE)
            {
               /* compound statement... */
            }
            else
            {
                /* error: */
            }
        break;
    }
}

void parse_compound_statement(struct parser_t *parser, struct bytecode_buffer_t *bytecode_buffer)
{
    push_scope(parser);

    /* {... */
    advance_token(parser);

    while(parser->current_token->token_type != TOKEN_PUNCTUATOR ||
          parser->current_token->token_name != TOKEN_PUNCTUATOR_CBRACE)
    {
        advance_token(parser);
    }

    pop_scope(parser);
}


void parse_selection_statement(struct parser_t *parser, struct bytecode_buffer_t *bytecode_buffer)
{

}

void parse_expression_statement(struct parser_t *parser, struct bytecode_buffer_t *bytecode_buffer)
{

}

void parse_iteration_statement(struct parser_t *parser, struct bytecode_buffer_t *bytecode_buffer)
{

}

void parse_jump_statement(struct parser_t *parser, struct bytecode_buffer_t *bytecode_buffer)
{

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

                    param_or_field = function->args;

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

                        param_or_field = (struct reference_type_t *)param_or_field->base.next;

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











