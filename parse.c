#include "parse.h"
#include "type.h"
#include "tok.h"
#include "scope.h"
#include "obj.h"
#include "exp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

char *type_specifier_strings[] = {
    [DECL_SPEC_INT]        = "int",
    [DECL_SPEC_SHORT]      = "short",
    [DECL_SPEC_CHAR]       = "char",
    [DECL_SPEC_FLOAT]      = "float",
    [DECL_SPEC_DOUBLE]     = "double",
    [DECL_SPEC_LONG]       = "long",
    [DECL_SPEC_UNSIGNED]   = "unsigned",
    [DECL_SPEC_SIGNED]     = "signed",
    [DECL_SPEC_VOID]       = "void",
    [DECL_SPEC_STRUCT]     = "struct",
    [DECL_SPEC_UNION]      = "union",
    // [TYPE_SPECIFIER_FUNCTION]   = "function",
    // TYPE_SPECIFIER_ARRAY,
    // TYPE_SPECIFIER_LINK,              /* this is only here to make the implementation easier...  */
    // TYPE_SPECIFIER_UNKNOWN,
};

uint32_t decl_specifier_lut[] = {
    [TOKEN_KEYWORD_INT]         = DECL_SPEC_INT,
    [TOKEN_KEYWORD_SHORT]       = DECL_SPEC_SHORT,
    [TOKEN_KEYWORD_CHAR]        = DECL_SPEC_CHAR,
    [TOKEN_KEYWORD_FLOAT]       = DECL_SPEC_FLOAT,
    [TOKEN_KEYWORD_DOUBLE]      = DECL_SPEC_DOUBLE,
    [TOKEN_KEYWORD_LONG]        = DECL_SPEC_LONG,
    [TOKEN_KEYWORD_UNSIGNED]    = DECL_SPEC_UNSIGNED,
    [TOKEN_KEYWORD_SIGNED]      = DECL_SPEC_SIGNED,
    [TOKEN_KEYWORD_VOID]        = DECL_SPEC_VOID,
    [TOKEN_KEYWORD_STRUCT]      = DECL_SPEC_STRUCT,
    [TOKEN_KEYWORD_UNION]       = DECL_SPEC_UNION,

    [TOKEN_KEYWORD_CONST]       = DECL_SPEC_CONST,
    [TOKEN_KEYWORD_RESTRICT]    = DECL_SPEC_RESTRICT,
    [TOKEN_KEYWORD_VOLATILE]    = DECL_SPEC_VOLATILE,

    [TOKEN_KEYWORD_TYPEDEF]     = DECL_SPEC_TYPEDEF,
    [TOKEN_KEYWORD_EXTERN]      = DECL_SPEC_EXTERN,
    [TOKEN_KEYWORD_STATIC]      = DECL_SPEC_STATIC,
    [TOKEN_KEYWORD_AUTO]        = DECL_SPEC_AUTO,
    [TOKEN_KEYWORD_REGISTER]    = DECL_SPEC_REGISTER
};

// uint32_t type_specifier_lut[] = {
//     [TOKEN_KEYWORD_INT]         = DECL_SPEC_INT,
//     [TOKEN_KEYWORD_SHORT]       = DECL_SPEC_SHORT,
//     [TOKEN_KEYWORD_CHAR]        = DECL_SPEC_CHAR,
//     [TOKEN_KEYWORD_FLOAT]       = DECL_SPEC_FLOAT,
//     [TOKEN_KEYWORD_DOUBLE]      = DECL_SPEC_DOUBLE,
//     [TOKEN_KEYWORD_LONG]        = DECL_SPEC_LONG,
//     [TOKEN_KEYWORD_UNSIGNED]    = DECL_SPEC_UNSIGNED,
//     [TOKEN_KEYWORD_SIGNED]      = DECL_SPEC_SIGNED,
//     [TOKEN_KEYWORD_VOID]        = DECL_SPEC_VOID,
//     [TOKEN_KEYWORD_STRUCT]      = DECL_SPEC_STRUCT,
//     [TOKEN_KEYWORD_UNION]       = DECL_SPEC_UNION,
// };

// uint32_t type_qualifier_lut[] = {
//     [TOKEN_KEYWORD_CONST]       = DECL_SPEC_CONST,
//     [TOKEN_KEYWORD_RESTRICT]    = DECL_SPEC_RESTRICT,
//     [TOKEN_KEYWORD_VOLATILE]    = DECL_SPEC_VOLATILE,
// };

// uint32_t storage_class_specifier_lut[] = {
//     [TOKEN_KEYWORD_TYPEDEF]     = DECL_SPEC_TYPEDEF,
//     [TOKEN_KEYWORD_EXTERN]      = DECL_SPEC_EXTERN,
//     [TOKEN_KEYWORD_STATIC]      = DECL_SPEC_STATIC,
//     [TOKEN_KEYWORD_AUTO]        = DECL_SPEC_AUTO,
//     [TOKEN_KEYWORD_REGISTER]    = DECL_SPEC_REGISTER
// };


int is_type_specifier(struct token_t *token)
{
    return token->type == TOKEN_KEYWORD && 
        token->name >= TOKEN_KEYWORD_FIRST_TYPE_SPECIFIER && 
        token->name <= TOKEN_KEYWORD_LAST_TYPE_SPECIFIER;
}

int is_type_qualifier(struct token_t *token)
{
    if(token->type == TOKEN_KEYWORD)
    {
        switch(token->name)
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
    return token->type == TOKEN_KEYWORD && 
        token->name >= TOKEN_KEYWORD_FIRST_STORAGE_CLASS_SPECIFIER &&
        token->name <= TOKEN_KEYWORD_LAST_STORAGE_CLASS_SPECIFIER;
    // if(token->type == TOKEN_KEYWORD)
    // {
    //     switch(token->name)
    //     {
    //         case TOKEN_KEYWORD_TYPEDEF:
    //         case TOKEN_KEYWORD_EXTERN:
    //         case TOKEN_KEYWORD_STATIC:
    //         case TOKEN_KEYWORD_AUTO:
    //         case TOKEN_KEYWORD_REGISTER:
    //             return 1;
    //     }
    // }

    // return 0;
}

int is_declaration_specifier(struct token_t *token)
{
    return is_storage_class_specifier(token) ||
           is_type_qualifier(token) ||
           is_type_specifier(token);
}

int is_typedef_name(struct token_t *token)
{
    return 0;
}

uint32_t type_specifier_from_token(struct token_t *token)
{
    if(is_type_specifier(token))
    {
        return decl_specifier_lut[token->name];
    }

    return DECL_SPEC_UNKNOWN;
}

// char *type_string(struct base_type_t *type)
// {
//     switch(type->type)
//     {
//         case TYPE_INT:
//             return "int";
//         break;

//         case TYPE_SHORT:
//             return "short";
//         break;

//         case TYPE_CHAR:
//             return "char";
//         break;

//         case TYPE_FLOAT:
//             return "float";
//         break;

//         case TYPE_DOUBLE:
//             return "double";
//         break;

//         case TYPE_LONG:
//             return "long";
//         break;

//         case TYPE_UNSIGNED:
//             return "unsigned";
//         break;

//         case TYPE_SIGNED:
//             return "signed";
//         break;

//         case TYPE_VOID:
//             return "void";
//         break;

//         case TYPE_STRUCT:
//             return "struct";
//         break;

//         case TYPE_UNION:
//             return "union";
//         break;

//         case TYPE_POINTER:
//             return "pointer";
//         break;

//         case TYPE_FUNCTION:
//             return "function";
//         break;

//         case TYPE_ARRAY:
//             return "array";
//         break;

//         case TYPE_IDENTIFIER:
//             return "identifier";
//         break;

//         case TYPE_UNKNOWN:
//         default:
//             return "unknown";
//         break;
//     }

// }

void advance_token(struct parser_t *parser)
{
    if(!parser->cur_token)
    {
        parser->cur_token = parser->tokens;
    }
    else if(parser->cur_token->type != TOKEN_EOF)
    {
        parser->cur_token = parser->cur_token->next;
    }

    parser->next_token = parser->cur_token->next;
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

    struct link_type_t *link;
    struct link_type_t *args;
    struct link_type_t *last_arg;
    struct function_type_t *function;
    struct aggretage_type_t *aggretage;

    struct aggretage_type_t *aggretage_type;
    struct function_type_t *function_type;
    struct link_type_t *link_type;

    int size;

    // while(type)
    // {
    //     switch(type->type)
    //     {
    //     case TYPE_INT:
    //         case TYPE_SHORT:
    //         case TYPE_CHAR:
    //         case TYPE_FLOAT:
    //         case TYPE_DOUBLE:
    //         case TYPE_LONG:
    //         case TYPE_UNSIGNED:
    //         case TYPE_CONST:
    //         case TYPE_VOLATILE:
    //         case TYPE_RESTRICT:
    //         case TYPE_EXTERN:
    //         case TYPE_SIGNED:
    //         case TYPE_VOID:
    //         case TYPE_POINTER:
    //             size = sizeof(struct base_type_t);
    //         break;

    //         case TYPE_STRUCT:
    //         case TYPE_UNION:
    //             size = sizeof(struct aggretage_type_t);
    //         break;

    //         case TYPE_FUNCTION:
    //             size = sizeof(struct function_type_t);
    //         break;

    //         case TYPE_ARRAY:
    //             size = sizeof(struct array_type_t);
    //         break;

    //         case TYPE_TYPEDEF:
    //             size = sizeof(struct typedef_type_t);
    //         break;

    //         case TYPE_IDENTIFIER:
    //             size = sizeof(struct identifier_type_t);
    //         break;

    //         case TYPE_LINK:
    //             size = sizeof(struct link_type_t);
    //         break;
    //     }

    //     new_type = calloc(size, 1);
    //     memcpy(new_type, type, size);
    //     new_type->next = NULL;

    //     if(new_type->type == TYPE_FUNCTION)
    //     {
    //         function_type = (struct function_type_t *)new_type;
    //         function = (struct function_type_t *)type;
    //         function_type->args = (struct link_type_t *)copy_type((struct base_type_t *)function->args);
    //     }
    //     else if(new_type->type == TYPE_LINK)
    //     {
    //         link_type = (struct link_type_t *)new_type;
    //         link = (struct link_type_t *)type;
    //         link_type->type = copy_type(link->type);
    //     }

    //     if(!first_type)
    //     {
    //         first_type = new_type;
    //     }
    //     else
    //     {
    //         last_type->next = new_type;
    //     }

    //     last_type = new_type;

    //     type = type->next;
    // }

    return first_type;
}

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/



#define SCOPE_MAX_DEPTH 8192
#define TEXT_BUFFER_SIZE 8192

void parse_tokens(struct token_t *tokens)
{
//    struct parser_t parser;
//
//    struct base_type_t *types;
//    struct link_type_t *link;
//
//    memset(&parser, 0, sizeof(struct parser_t));
//
//    parser.tokens = tokens;
//    parser.cur_token = tokens;
//
//    parser.scope_stack_top = -1;
//    parser.scope_stack = calloc(sizeof(struct scope_t *), SCOPE_MAX_DEPTH);
//
////    parser.global_bytecode.buffer_size = 1024;
////    parser.global_bytecode.bytecode = calloc(parser.global_bytecode.buffer_size, 1);
//
//    parser.reg_index = 0;
//
//    parser.declaration_depth = -1;
//    /* global (file) scope... */
//    push_scope(&parser);
//
////    types = parse_declaration(&parser, 0);
////    link = calloc(1, sizeof(struct link_type_t));
////    link->base.type = TYPE_LINK;
////    link->type = types;
////    translate_type(link, 0);
//
//    while(parser.cur_token->type != TOKEN_EOF)
//    {
////        if(is_type_specifier(parser.cur_token))
////        {
////            types = (struct reference_type_t *)parse_declaration(&parser);
////        }
////        else
////        {
////            advance_token(&parser);
////        }
//
//        //exp_16(&parser);
//        if(is_declaration_specifier(parser.cur_token))
//        {
//            parse_declaration(&parser, 0);
//        }
//        else
//        {
//            parse_expression_statement(&parser);
//        }
//    }
//
//    pop_scope(&parser);
}

void error(uint32_t line, uint32_t column, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("ERROR (%u : %u): ", line, column);
    vprintf(format, args);
    printf("\n");
    exit(-1);
}

void parse(char *text)
{
    struct parser_t parser = {};
    parser.tokens = lex_tokens(text);
    // advance_token(&parser);
    // push_scope(&parser);

    do
    {
        parse_statement(&parser);
    }
    while(parser.cur_token->type != TOKEN_EOF);
    
    // parse_statement(&parser);
    // pop_scope(&parser);
}

struct declarator_t *parse_declaration(struct parser_t *parser, uint32_t in_arg_list)
{
    // struct base_type_t *type = NULL;
    // struct base_type_t *last_type = NULL;

//     struct base_type_t *specifiers_qualifiers = NULL;
//     struct base_type_t *last_specifier_qualifier = NULL;

//     struct base_type_t *new_type = NULL;
//     struct base_type_t *current_type = NULL;
// //    struct base_type_t *prev_type = NULL;
//     struct base_type_t *declarator = NULL;
//     struct base_type_t *temp_type = NULL;

//     struct reference_type_t *params;
//     struct reference_type_t *next_param;

//     struct function_type_t *function_type;

//     struct object_t *object;struct declarator_t *parse_declarator(struct parser_t *parser, struct declarator_t *declarator);
//     int old_function_style;
//     struct identifier_type_t *identifier;
//     struct token_t *next_token;
//     struct link_type_t *link = NULL;
//     struct link_type_t *prev_link;
    struct declarator_t *declarators = NULL;
    struct declarator_t *last_declarator = NULL;
    parser->declaration_depth++;

    in_arg_list = in_arg_list && 1;
    //if(is_type_specifier(parser->cur_token))
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

    struct type_t *type = NULL;

    if(is_declaration_specifier(parser->cur_token))
    {
        struct decl_spec_t *specifiers = NULL;
        struct decl_spec_t *last_specifier = NULL;
        uint32_t found_type_specifiers = 0;

        while(is_declaration_specifier(parser->cur_token))
        {
            struct decl_spec_t *specifier = calloc(1, sizeof(struct decl_spec_t));

            specifier->specifier = decl_specifier_lut[parser->cur_token->name];

            if(specifiers == NULL)
            {
                specifiers = specifier;
            }
            else
            {
                last_specifier->next = specifier;
            }

            last_specifier = specifier;

            if(specifier->specifier == DECL_SPEC_STRUCT || specifier->specifier == DECL_SPEC_UNION)
            {
                if(found_type_specifiers)
                {
                    /* error: some tomfoolery going on with them specifiers... */
                }

                type = parse_aggregate_declaration(parser);

                break;
            }

            found_type_specifiers |= is_type_specifier(parser->cur_token);
            advance_token(parser);
        }

        if(type == NULL)
        {
            type = calloc(1, sizeof(struct type_t));
            type->type = TYPE_PRIMITIVE;
        }

        type->specifiers = specifiers;
    }
    // else if(parser->cur_token->type == TOKEN_IDENTIFIER)
    // {
    //     /* this token is something other than any valid declaration
    //     specifier, so test it to see if it's a typedef'd name */
    //     if(is_typedef_name(parser->cur_token))
    //     {

    //     }
    //     else
    //     {
    //         /* nope, not a typedef */
    //         /* error: unknown type */
    //     }
    // }

    // current_type = specifiers_qualifiers;
    // uint32_t done = 0;
    while(1)
    {
        /* declarator: identifier, function, array, pointer, etc... */
        struct declarator_t *declarator = parse_declarator(parser, NULL);

        struct type_t *declarator_type = declarator->type;

        if(declarator_type != NULL)
        {
            while(declarator_type->next != NULL)
            {
                declarator_type = declarator_type->next;
            }

            declarator_type->next = type;
        }
        else
        {
            declarator->type = type;
        }

        if(declarators == NULL)
        {
            declarators = declarator;
        }
        else
        {
            last_declarator->next = declarator;
        }

        last_declarator = declarator;

        if(parser->cur_token->type == TOKEN_PUNCTUATOR)
        {
            if(parser->cur_token->name == TOKEN_PUNCTUATOR_EQUAL)
            {
                if(in_arg_list)
                {
                    /* error: can't assign to function parameter */
                    error(parser->cur_token->line, parser->cur_token->column, "Can't assign to function parameter");
                }
                /* handle assignment */
            }

            if(parser->cur_token->name == TOKEN_PUNCTUATOR_COMMA)
            {
                if(in_arg_list)
                {
                    /* we're currently parsing the argument list of a function. However, paramaters need to 
                    have a full declaration, and so we need to gtfo here so the caller can handle the ',' token
                    and call us again, so we can apply the parsing for an entire declaration. */
                    break;
                }
                else
                {
                    advance_token(parser);

                    // if(is_declaration_specifier(parser->cur_token))
                    if(parser->cur_token->type != TOKEN_IDENTIFIER && !is_type_qualifier(parser->cur_token))
                    {
                        error(parser->cur_token->line, parser->cur_token->column, "Expected declarator");
                    }
                }
            }
            else if(!in_arg_list && parser->cur_token->name == TOKEN_PUNCTUATOR_SEMICOLON)
            {
                /* end of declaration */
                advance_token(parser);
                if(declarator->type != NULL && declarator->type->type == TYPE_FUNCTION)
                {
                    declarator->type->func.prototype = 1;
                }
                break;
            }
            else if(in_arg_list && parser->cur_token->name == TOKEN_PUNCTUATOR_CPARENTHESIS)
            {
                /* end of function parameter list */
                break;
            }
            else if(parser->cur_token->name == TOKEN_PUNCTUATOR_OBRACE)
            {
                /* function body */
                break;
            }
            else
            {
                /* error: expecting ',', ';' or '=' */
            }
        }


        // if(!is_in_arg_list)
        // {
        //     if(parser->cur_token->type == TOKEN_PUNCTUATOR)
        //     {
        //         if(parser->cur_token->name == TOKEN_PUNCTUATOR_ASSIGN)
        //         {
        //             /* deal with assignment, call the expression routines... */
        //         }

        //         if(parser->cur_token->name == TOKEN_PUNCTUATOR_SEMICOLON)
        //         {
        //             advance_token(parser);
        //             break;
        //         }
        //     }
        //     else
        //     {
        //         /* error: expecting '=', ',' or ';'... */
        //     }

        //     if(parser->cur_token->name == TOKEN_PUNCTUATOR_COMMA)
        //     {
        //         specifiers_qualifiers = copy_type(specifiers_qualifiers);
        //     }
        //     else
        //     {
        //         /* error: some non-sense going on... */
        //     }
        // }

        // advance_token(parser);
    }

    // if(declarator)


    parser->declaration_depth--;

    return declarators;
}

struct declarator_t *parse_declarator(struct parser_t *parser, struct declarator_t *declarator)
{
    // struct base_type_t *type = NULL;
    // struct base_type_t *ret_declarator = NULL;
    // struct base_type_t *temp = NULL;
    // struct base_type_t *new_type = NULL;

    // struct function_type_t *function_type = NULL;
    // struct array_type_t *array_type = NULL;
    // struct base_type_t *pointer_type = NULL;
    // struct identifier_type_t *identifier = NULL;
    // struct link_type_t *params_or_fields = NULL;
    // struct link_type_t *param_or_field = NULL;
    // struct link_type_t *last_param_or_field = NULL;

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


//    if(is_type_specifier(parser->cur_token))
//    {
//        /* welp, this is an error... */
//    }

    if(declarator == NULL)
    {
        declarator = calloc(1, sizeof(struct declarator_t ));
    }

    struct type_t *type = NULL;

    /* optional pointer before the direct-declarator... */
    if(parser->cur_token->type == TOKEN_PUNCTUATOR)
    {
        if(parser->cur_token->name == TOKEN_PUNCTUATOR_ASTERISC)
        {
            type = calloc(1, sizeof(struct type_t));
            declarator->type = type;
            type->type = TYPE_POINTER;
            while(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_ASTERISC)
            {
                struct pointer_t *pointer = calloc(1, sizeof(struct pointer_t));

                if(type->pointer == NULL)
                {
                    type->pointer = pointer;
                }
                else
                {
                    type->last_pointer->next = pointer;
                }

                type->last_pointer = pointer;

                while(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_ASTERISC)
                {
                    pointer->count++;
                    advance_token(parser);
                }

                while(is_type_qualifier(parser->cur_token))
                {
                    struct decl_spec_t *qualifier = calloc(1, sizeof(struct decl_spec_t));
                    qualifier->specifier = decl_specifier_lut[parser->cur_token->name];
                    qualifier->next = pointer->qualifiers;
                    pointer->qualifiers = qualifier;
                    advance_token(parser);
                }
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
    switch(parser->cur_token->type)
    {
        case TOKEN_IDENTIFIER:
            declarator->identifier = strdup(parser->cur_token->constant.string_constant);
            advance_token(parser);
        break;

        case TOKEN_PUNCTUATOR:
            if(parser->cur_token->name == TOKEN_PUNCTUATOR_OPARENTHESIS)
            {
                /* declarator is inside parenthesis, which means anything inside of it has precedence. If
                there are any '*', for example, the declarator will have pointer to something type */
                advance_token(parser);
                struct type_t *next_type = declarator->type;
                declarator->type = NULL;
                parse_declarator(parser, declarator);

                if(declarator->type != NULL)
                {
                    /* there was '*' and type qualifiers inside the parenthesis, so the declarator type
                    is now pointer to whatever we find when we reach the end of this call */
                    struct type_t *last_type = declarator->type;

                    while(last_type->next)
                    {
                        last_type = last_type->next;
                    }

                    last_type->next = next_type;
                }

                if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                {
                    advance_token(parser);
                }
                else
                {
                    /* error: expecting ')' */
                    error(parser->cur_token->line, parser->cur_token->column, "Expecting '(' token");
                }
            }
            else
            {
                /* everything else is wrong... */
                error(parser->cur_token->line, parser->cur_token->column, "Everything else is wrong");
            }
        break;
    }





    if(parser->cur_token->type == TOKEN_PUNCTUATOR)
    {
        // if(declarator->type == NULL)
        // {
        //     declarator->type = calloc(1, sizeof(struct type_t));
        //     type = declarator->type;
        // }

        struct type_t temp_type = {.type = TYPE_UNKNOWN};

        // struct type_t *type = declarator->type;
        /* this declarator may be a function or an array... */
        switch(parser->cur_token->name)
        {
            case TOKEN_PUNCTUATOR_OPARENTHESIS:
                temp_type.type = TYPE_FUNCTION;
                /* ( */
                advance_token(parser);

                /* this is the parameter list of a function, which means we'll be
                possibly declaring other identifiers, so we call the declaration
                parsing routine here...*/
                //parser->param_list_level++;

                // function_type->args = 0;
                temp_type.func.arg_count = 0;

                struct declarator_t *args = NULL;
                struct declarator_t *last_arg = NULL;

                while(1)
                {
                    /* parse parameter-list... */
                    struct declarator_t *arg = parse_declaration(parser, 1);

                    if(arg)
                    {
                        temp_type.func.arg_count++;

                        if(args == NULL)
                        {
                            args = arg;
                        }
                        else
                        {
                            last_arg->next = arg;
                        }

                        last_arg = arg;
                    }

                    if(parser->cur_token->type == TOKEN_PUNCTUATOR)
                    {
                        if(parser->cur_token->name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                        {
                            break;
                        }
                        else if(parser->cur_token->name == TOKEN_PUNCTUATOR_COMMA)
                        {
                            advance_token(parser);

                            if(!is_declaration_specifier(parser->cur_token))
                            {
                                /* function parameters require declaration specifiers */
                                error(parser->cur_token->line, parser->cur_token->column, "Expecting declaration specifiers");
                            }
                        }
                        else
                        {
                            /* error: unexpected token. Expecting ')' or ','... */
                            error(parser->cur_token->line, parser->cur_token->column, "Unexpected token. Expecting ')' or','");
                        }
                    }
                }

                temp_type.func.args = args;

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
                // array_type = calloc(sizeof(struct array_type_t), 1);

                /* [... */
                advance_token(parser);
            break;

            default:
                /* anything else is wrong... */
            break;
        }

        if(temp_type.type != TYPE_UNKNOWN)
        {
            if(type == NULL)
            {
                declarator->type = calloc(1, sizeof(struct type_t));
                type = declarator->type;
            }

            type->type = temp_type.type;

            switch(temp_type.type)
            {
                case TYPE_STRUCT:
                case TYPE_UNION:
                    type->aggregate = temp_type.aggregate;
                break;

                case TYPE_ARRAY:
                    type->array = temp_type.array;
                break;

                case TYPE_FUNCTION:
                    type->func = temp_type.func;
                break;
            }
        }
    }
    else
    {
        /* anything else is wrong... */
    }




    // if(function_type)
    // {
    //     type = (struct base_type_t *) function_type;

    //     if(pointer_type)
    //     {
    //         /* function that returns pointer to... */
    //         function_type->base.next = pointer_type;
    //     }
    // }
    // else
    // {
    //     /* pointer to... */
    //     type = pointer_type;
    // }

    // if(identifier)
    // {
    //     /* identifier is a function/array of/pointer to/nothing... */
    //     identifier->base.next = type;
    //     type = (struct base_type_t *)identifier;
    // }

    // if(ret_declarator)
    // {
    //     temp = ret_declarator;

    //     while(temp)
    //     {
    //         /* we skip until the end of the list, since we'll be
    //         appending what we found in this call to what was
    //         found in the recursive call.

    //         If this is a function, the last element of the list will
    //         be the function type, since its return type is kept in the
    //         next pointer, and up to this point we still don't know its
    //         return type (since it was found in the current call)... */
    //         temp = temp->next;
    //     }

    //     switch(temp->type)
    //     {
    //         case TYPE_FUNCTION:

    //             if(function_type)
    //             {
    //                 /* error: function returning function... */
    //             }

    //             if(array_type)
    //             {
    //                 /* error: function returning array... */
    //             }

    //             function_type = (struct function_type_t *)temp;

    //         break;
    //     }

    //     temp->next = type;

    //     type = ret_declarator;
    // }

    return declarator;
}


struct type_t *parse_aggregate_declaration(struct parser_t *parser)
{
    // struct type_t *type;

    // struct base_type_t *field_type = NULL;
    // struct link_type_t *field = NULL;
    // struct link_type_t *fields = NULL;
    // struct link_type_t *last_field = NULL;

    struct type_t *type = calloc(sizeof(struct type_t), 1);
    type->type = type_specifier_from_token(parser->cur_token);

    // switch(specifier)
    // {
    //     case DECL_SPEC_STRUCT:

    //     break;

    //     case DECL_SPEC_UNION:

    //     break;
    // }

    // if(parser->cur_token->type == TOKEN_KEYWORD)
    // {
    //     switch(parser->cur_token->name)
    //     {
    //         case TOKEN_KEYWORD_STRUCT:
    //             type->base.type = TYPE_STRUCT;
    //         break;

    //         case TOKEN_KEYWORD_UNION:
    //             type->base.type = TYPE_UNION;
    //         break;
    //     }
    // }
    // else
    // {
    //     /* everything else is wrong... */
    // }

    advance_token(parser);

    if(parser->cur_token->type == TOKEN_IDENTIFIER)
    {
        type->aggregate.identifier = strdup(parser->cur_token->constant.string_constant);
        advance_token(parser);
    }

    if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_OBRACE)
    {
        /* struct/union (identifier) { ... */
        advance_token(parser);

        struct declarator_t *fields = NULL;
        struct declarator_t *last_field = NULL;

        while(1)
        {
            /* parse the fields, which can be structs as well */
            struct declarator_t *field = parse_declaration(parser, 0);
            // struct type_t *last_field = field;

            if(fields == NULL)
            {
                fields = field;
            }
            else
            {
                last_field->next = field;
            }

            while(field->next != NULL)
            {
                field = field->next;
                type->aggregate.field_count++;
            }

            last_field = field;

            if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_CBRACE)
            {
                advance_token(parser);
                break;
            }
        }

        type->aggregate.fields = fields;
    }

    if(type->aggregate.identifier != NULL)
    {
        /* this aggregate type has a tag, so we stash it... */
        // stash_aggregate_type(parser, type);
    }

    return type;
}

void parse_statement(struct parser_t *parser)
{
    struct token_t *token;

    if(parser->current_scope == NULL)
    {
        parse_compound_statement(parser);
    }
    else if(parser->cur_token->type == TOKEN_IDENTIFIER && parser->next_token->type == TOKEN_PUNCTUATOR && 
            parser->next_token->type == TOKEN_PUNCTUATOR_COLON)
    {
        parse_labeled_statement(parser);
    }
    else if(parser->cur_token->type == TOKEN_KEYWORD)
    {
        switch(parser->cur_token->name)
        {
            case TOKEN_KEYWORD_CASE:
            case TOKEN_KEYWORD_DEFAULT:
                parse_labeled_statement(parser);
            break;
        }
    }
    else if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_OBRACE)
    {
        parse_compound_statement(parser);
    }
    else
    {
        parse_expression_statement(parser);
    }
}

void parse_labeled_statement(struct parser_t *parser)
{
    switch(parser->cur_token->type)
    {
        case TOKEN_IDENTIFIER:
        {
            advance_token(parser);
            if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_COLON)
            {
                /* create label */
                advance_token(parser);
                parse_statement(parser);
            }
            else
            {
                /* error: expecting ':' after identifier */
            }
        }
        break;

        case TOKEN_KEYWORD:
        {
            switch(parser->cur_token->name)
            {
                case TOKEN_KEYWORD_CASE:
                    advance_token(parser);
                    if(parser->cur_token->type == TOKEN_CONSTANT)
                    {
                        advance_token(parser);

                        if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_COLON)
                        {
                            advance_token(parser);
                            /* create case */
                            parse_statement(parser);
                        }
                    }
                break;

                case TOKEN_KEYWORD_DEFAULT:
                {
                    advance_token(parser);

                    if(parser->cur_token->type == TOKEN_PUNCTUATOR && parser->cur_token->name == TOKEN_PUNCTUATOR_COLON)
                    {
                        /* default case */
                        advance_token(parser);
                        parse_statement(parser);
                    }
                }
                break;
            }
        }
        break;
    }
}

void parse_compound_statement(struct parser_t *parser)
{
    push_scope(parser);

    /* {... */
    advance_token(parser);

    while((parser->cur_token->type != TOKEN_PUNCTUATOR || parser->cur_token->name != TOKEN_PUNCTUATOR_CBRACE) && 
            parser->cur_token->type != TOKEN_EOF)
    {
        if(is_declaration_specifier(parser->cur_token))
        {
            struct declarator_t *declarator = parse_declaration(parser, 0);

            if(declarator->type->type == TYPE_FUNCTION && parser->current_scope->parent != NULL)
            {
                error(parser->cur_token->line, parser->cur_token->column, "Functions may be declared only in file scope");

                if(!declarator->type->func.prototype)
                {
                    if(parser->cur_token->type != TOKEN_PUNCTUATOR || parser->cur_token->name != TOKEN_PUNCTUATOR_OBRACE)
                    {
                        error(parser->cur_token->line, parser->cur_token->column, "Expecting '{' token");
                    }
                    parser->cur_function = declarator;
                    parse_compound_statement(parser);
                    parser->cur_function = NULL;
                }
            }
        }
        else
        {
            parse_statement(parser);
        }
    }

    pop_scope(parser);

    if(parser->current_scope != NULL)
    {
        if(parser->cur_token->type != TOKEN_PUNCTUATOR || parser->cur_token->name != TOKEN_PUNCTUATOR_CBRACE)
        {
            /* error: expecting '}' */
        }

        advance_token(parser);
    }
}


void parse_selection_statement(struct parser_t *parser)
{

}

void parse_iteration_statement(struct parser_t *parser)
{

}

void parse_jump_statement(struct parser_t *parser)
{

}

void parse_expression_statement(struct parser_t *parser)
{
    struct base_exp_node_t *exp;
    exp = parse_exp(parser);

    if(parser->cur_token->type != TOKEN_PUNCTUATOR || parser->cur_token->name != TOKEN_PUNCTUATOR_SEMICOLON)
    {
        error(parser->cur_token->line, parser->cur_token->column, "Expecting ';");
    }

    /* ';' */
    advance_token(parser);

    traverse_exp_tree(exp);
    printf("\n");
//    translate_expression(expression);
}

void translate_type(struct link_type_t *type, int single_reference)
{
    // struct identifier_type_t *identifier;
    // struct function_type_t *function;
    // struct link_type_t *param_or_field;
    // struct base_type_t *base_type;
    // struct aggretage_type_t *aggretage_type;

    // while(type)
    // {
    //     base_type = type->type;

    //     while(base_type)
    //     {
    //         switch(base_type->type)
    //         {
    //             case TYPE_IDENTIFIER:
    //                 identifier = (struct identifier_type_t *)base_type;
    //                 printf("[%s] is ", identifier->identifier);
    //             break;

    //             case TYPE_POINTER:
    //                 printf("a pointer to ");
    //             break;

    //             case TYPE_FUNCTION:
    //                 function = (struct function_type_t *)base_type;

    //                 printf("a function (");

    //                 param_or_field = function->args;

    //                 while(param_or_field)
    //                 {
    //                     translate_type(param_or_field, 1);

    //                     param_or_field = (struct link_type_t *)param_or_field->base.next;

    //                     if(param_or_field)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }

    //                 printf("), returning ");
    //             break;

    //             case TYPE_STRUCT:
    //                 aggretage_type = (struct aggretage_type_t *)base_type;

    //                 if(aggretage_type->name)
    //                 {
    //                     printf("a struct [%s] {", aggretage_type->name);
    //                 }
    //                 else
    //                 {
    //                     printf("an anonymous struct {");
    //                 }

    //                 param_or_field = aggretage_type->fields;

    //                 while(param_or_field)
    //                 {
    //                     translate_type(param_or_field, 1);

    //                     param_or_field = (struct link_type_t *)param_or_field->base.next;

    //                     if(param_or_field)
    //                     {
    //                         printf(", ");
    //                     }
    //                 }

    //                 printf("}, ");


    //             break;

    //             case TYPE_INT:
    //                 printf("an int");
    //             break;

    //             case TYPE_FLOAT:
    //                 printf("a float");
    //             break;

    //             case TYPE_DOUBLE:
    //                 printf("a double");
    //             break;

    //             case TYPE_VOID:
    //                 printf("void");
    //             break;

    //             case TYPE_SHORT:
    //                 printf("a short");
    //             break;

    //             case TYPE_CHAR:
    //                 printf("a char");
    //             break;

    //             default:
    //                 return;
    //             break;
    //         }

    //         base_type = base_type->next;
    //     }

    //     type = single_reference ? NULL : (struct link_type_t *)type->base.next;
    // }
}











