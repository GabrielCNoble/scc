#include "parse.h"
#include "type.h"
#include "tok.h"
#include "scope.h"
#include "obj.h"

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

static char identifier[8192];

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

uint32_t prim_type_size_lut[] = {
    [PRIM_TYPE_CHAR]    = sizeof(char),
    [PRIM_TYPE_UCHAR]   = sizeof(unsigned char),
    [PRIM_TYPE_SHORT]   = sizeof(short),
    [PRIM_TYPE_USHORT]  = sizeof(unsigned char),
    [PRIM_TYPE_INT]     = sizeof(int),
    [PRIM_TYPE_UINT]    = sizeof(unsigned int),
    [PRIM_TYPE_LONG]    = sizeof(long),
    [PRIM_TYPE_LLONG]   = sizeof(long long),
    [PRIM_TYPE_ULONG]   = sizeof(unsigned long),
    [PRIM_TYPE_ULLONG]  = sizeof(unsigned long long),
    [PRIM_TYPE_FLOAT]   = sizeof(float),
    [PRIM_TYPE_DOUBLE]  = sizeof(double),
    [PRIM_TYPE_LDOUBLE] = sizeof(long double),
};

int is_type_specifier(struct token_t *token)
{
    return token->type == TOKEN_KEYWORD && 
        token->name >= TOKEN_KEYWORD_FIRST_TYPE_SPECIFIER && 
        token->name <= TOKEN_KEYWORD_LAST_TYPE_SPECIFIER;
}

int is_type_qualifier(struct token_t *token)
{
    return token->type == TOKEN_KEYWORD && 
        token->name >= TOKEN_KEYWORD_FIRST_TYPE_QUALIFIER &&
        token->name <= TOKEN_KEYWORD_LAST_TYPE_QUALIFIER;
}

int is_storage_class_specifier(struct token_t *token)
{
    return token->type == TOKEN_KEYWORD && 
        token->name >= TOKEN_KEYWORD_FIRST_STORAGE_CLASS_SPECIFIER &&
        token->name <= TOKEN_KEYWORD_LAST_STORAGE_CLASS_SPECIFIER;
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

void advance_token(struct parser_t *parser)
{
    parser->cur_token = parser->next_token;
    parser->next_token = lex_token(parser->src, &parser->src_offset, &parser->line, &parser->column);
}

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

struct type_t *stash_aggregate_type(struct parser_t *parser, struct type_t *type)
{
    if(type->aggregate.identifier != NULL)
    {
        struct type_t *stashed_type = get_aggregate_type(parser, type->aggregate.identifier);

        if(stashed_type != NULL)
        {
            if(stashed_type != type)
            {
                if(stashed_type->complete)
                {
                    /* error: redefinition of aggregate type */
                }

                if(type->complete)
                {

                }
            }
        }
        else
        {
            type->aggregate.stored_next = parser->aggregate_types;
            parser->aggregate_types = type;   
        }
    }
}

struct type_t *get_aggregate_type(struct parser_t *parser, char *name)
{
    struct type_t *type = parser->aggregate_types;

    while(type)
    {
        if(!strcmp(name, type->aggregate.identifier))
        {
            break;
        }

        type = type->aggregate.stored_next;
    }

    return type;
}


// struct base_type_t *stash_typedef_type(struct parser_t *parser, struct base_type_t *type)
// {

// }


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
//    while(parser.cur_token.type != TOKEN_EOF)
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
    struct parser_t parser = {.src = text};
    /* prime next token. Current token will be initialized
    by the first call to parse_compound_statement */
    advance_token(&parser);
    parser.types = pool_CreateTyped(struct type_t, 4096);
    parser.declarators = pool_CreateTyped(struct declarator_t, 4096);
    parser.exp_nodes = pool_CreateTyped(struct exp_node_t, 4096);
    parser.statements = pool_CreateTyped(struct statement_t, 4096);
    parser.exp_trees = pool_CreateTyped(struct exp_tree_t, 4096);
    parser.scopes = pool_CreateTyped(struct scope_t, 4096);

    do
    {
        parse_statement(&parser); 
    }
    while(parser.cur_token.type != TOKEN_EOF);

    dump_program(&parser);
    pool_Destroy(&parser.types);
    pool_Destroy(&parser.declarators);
    pool_Destroy(&parser.exp_nodes);
    pool_Destroy(&parser.statements);
    pool_Destroy(&parser.exp_trees);
    pool_Destroy(&parser.scopes);
}

// uint32_t valid_type_specifier_lut[] = {
//     (1 << DECL_SPEC_CHAR),
//     (1 << DECL_SPEC_SHORT),
//     (1 << DECL_SPEC_INT),
//     (1 << DECL_SPEC_LONG),
//     (1 << DECL_SPEC_FLOAT),
//     (1 << DECL_SPEC_DOUBLE),
// };

struct 
{
    uint32_t        mask;
    const char *    msg;
} invalid_type_specifier_combinations_lut[] = {

    {.mask = (1 << DECL_SPEC_CHAR) | (1 << DECL_SPEC_SHORT),   .msg = "char and short in declaration specifier"},
    {.mask = (1 << DECL_SPEC_CHAR) | (1 << DECL_SPEC_INT),     .msg = "char and int in declaration specifier"},
    {.mask = (1 << DECL_SPEC_CHAR) | (1 << DECL_SPEC_FLOAT),   .msg = "char and float in declaration specifier"},
    {.mask = (1 << DECL_SPEC_CHAR) | (1 << DECL_SPEC_DOUBLE),  .msg = "char and double in declaration specifier"},

    // {.mask = (1 << DECL_SPEC_SHORT) | (1 << DECL_SPEC_INT),    .msg = "short and int in declaration specifier"},
    {.mask = (1 << DECL_SPEC_SHORT) | (1 << DECL_SPEC_FLOAT),  .msg = "short and float in declaration specifier"},
    {.mask = (1 << DECL_SPEC_SHORT) | (1 << DECL_SPEC_DOUBLE), .msg = "short and double in declaration specifier"},

    {.mask = (1 << DECL_SPEC_INT) | (1 << DECL_SPEC_FLOAT),    .msg = "int and float in declaration specifier"},
    {.mask = (1 << DECL_SPEC_INT) | (1 << DECL_SPEC_DOUBLE),   .msg = "int and double in declaration specifier"},

    {.mask = (1 << DECL_SPEC_FLOAT) | (1 << DECL_SPEC_DOUBLE), .msg = "float and double in declaration specifier"},

    {.mask = 0}
};

struct declarator_t *parse_declaration(struct parser_t *parser, uint32_t flags)
{
    struct declarator_t *declarators = NULL;
    struct declarator_t *last_declarator = NULL;
    parser->declaration_depth++;
    // in_arg_list = in_arg_list && 1;

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

    if(is_declaration_specifier(&parser->cur_token))
    {
        uint32_t specifiers = 0;
        uint32_t long_count = 0;

        while(is_declaration_specifier(&parser->cur_token))
        {
            uint32_t specifier = decl_specifier_lut[parser->cur_token.name];
            uint32_t cur_specifier = 1 << specifier;
            uint32_t cur_specifiers = specifiers | cur_specifier;

            if((specifiers & STORAGE_CLASS_SPEC_MASK) && (cur_specifier & STORAGE_CLASS_SPEC_MASK))
            {
                /* TODO: add exception for _Thread_local */
                error(parser->cur_token.line, parser->cur_token.column, "Only one storage class specifier is allowed per declaration");
            }

            if((cur_specifiers & PRIMITIVE_TYPE_SPEC_MASK) && (cur_specifiers & AGGREGATE_TYPE_SPEC_MASK))
            {
                error(parser->cur_token.line, parser->cur_token.column, "Can't have aggregate and primitive type specifiers in the same declaration. Make up your mind!");
            }

            for(uint32_t index = 0; invalid_type_specifier_combinations_lut[index].mask != 0; index++)
            {
                uint32_t mask = invalid_type_specifier_combinations_lut[index].mask;
                const char *msg = invalid_type_specifier_combinations_lut[index].msg;

                if((specifiers & PRIMITIVE_TYPE_SPEC_MASK) == cur_specifier)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Duplicate %s in declaration specifiers", type_specifier_strings[specifier]);
                }
                else if((cur_specifiers & mask) == mask)
                {
                    error(parser->cur_token.line, parser->cur_token.column, msg);
                }
            }

            if(specifiers & (1 << DECL_SPEC_LONG))
            {
                long_count++;
            }

            specifiers = cur_specifiers;

            if(specifier == DECL_SPEC_STRUCT || specifier == DECL_SPEC_UNION)
            {
                type = parse_aggregate_declaration(parser);
                break;
            }

            advance_token(parser);
        }

        if(type == NULL)
        {
            // type = calloc(1, sizeof(struct type_t));
            type = pool_AddElement(&parser->types, NULL);
            type->type = TYPE_PRIMITIVE;
            type->complete = 1;
        }

        type->specifiers = specifiers;
    }
    // else if(parser->cur_token.type == TOKEN_IDENTIFIER)
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

    if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_SEMICOLON)
    {
        /* end of declaration, no declarator */
        advance_token(parser);
    }
    else
    {
        while(1)
        {
            /* declarator: identifier, function, array, pointer, etc... */
            struct declarator_t *declarator = parse_declarator(parser, NULL, flags);

            if(declarator == NULL)
            {
                break;
            }

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

            if(flags & (PARSER_FLAG_ARG_LIST | PARSER_FLAG_TYPE_NAME))
            {
                /* we're parsing the parameter list of a function or a type name */
                if(parser->cur_token.type == TOKEN_PUNCTUATOR && 
                    (parser->cur_token.name == TOKEN_PUNCTUATOR_COMMA || 
                     parser->cur_token.name == TOKEN_PUNCTUATOR_CPARENTHESIS))
                {
                    /* next parameter, end of parameter list or end of type name. 
                    The the caller code will handle the ',' or the ')' */
                    break;
                }
                else
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expected ',' or ')'");
                }
            }
            else
            {
                /* we're handling a declaration, and possibly a definition */

                if(parser->declaration_depth == 1)
                {
                    if(declarator->identifier == NULL)
                    {
                        error(parser->cur_token.line, parser->cur_token.column, "Missing identifier");
                    }

                    create_object(parser, declarator);
                }

                if(parser->cur_token.type == TOKEN_PUNCTUATOR)
                {
                    if(parser->cur_token.name == TOKEN_PUNCTUATOR_EQUAL)
                    {
                        if(parser->declaration_depth == 1)
                        {

                        }
                        else
                        {
                            error(parser->cur_token.line, parser->cur_token.column, "Unexpected token '='");        
                        }
                    }
                    else if(parser->cur_token.name == TOKEN_PUNCTUATOR_COMMA)
                    {
                        /* multiple identifiers with the same "base" type */
                        advance_token(parser);
                    }
                    else if(parser->cur_token.name == TOKEN_PUNCTUATOR_SEMICOLON)
                    {
                        /* end of definition, without initialization */
                        advance_token(parser);
                        break;
                    }
                    else if(parser->cur_token.name == TOKEN_PUNCTUATOR_OBRACE)
                    {
                        /* probably the start of a function body, which the caller will handle */
                        break;
                    }
                    else
                    {
                        error(parser->cur_token.line, parser->cur_token.column, "Unexpected token");    
                    }
                }
                else
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expected ';', or '='");
                }
            }
        }
    }

    if(declarators != NULL && type == NULL)
    {
        /* we found declarators but no type specifiers */
        error(parser->cur_token.line, parser->cur_token.column, "Expecting declaration specifiers");
    }

    parser->declaration_depth--;

    return declarators;
}

struct declarator_t *parse_declarator(struct parser_t *parser, struct declarator_t *declarator, uint32_t flags)
{
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
        declarator = pool_AddElement(&parser->declarators, NULL);
    }

    struct type_t *type = NULL;
    struct type_t *last_pointer = NULL;

    /* optional pointer before the direct-declarator... */
    if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    {
        if(parser->cur_token.name == TOKEN_PUNCTUATOR_ASTERISC)
        {   
            do
            {
                while(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_ASTERISC)
                {
                    // pointer->count++;
                    struct type_t *pointer = pool_AddElement(&parser->types, NULL);
                    pointer->type = TYPE_POINTER;

                    if(type == NULL)
                    {
                        type = pointer;
                    }
                    else
                    {
                        last_pointer->next = pointer;
                    }

                    last_pointer = pointer;

                    advance_token(parser);
                }

                while(is_type_qualifier(&parser->cur_token))
                {
                    // struct decl_spec_t *qualifier = calloc(1, sizeof(struct decl_spec_t));
                    // qualifier->specifier = decl_specifier_lut[parser->cur_token.name];
                    // qualifier->next = pointer->qualifiers;
                    // pointer->qualifiers = qualifier;
                    last_pointer->specifiers |= 1 << decl_specifier_lut[parser->cur_token.name];
                    advance_token(parser);
                }
            }
            while(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_ASTERISC);
        }
        // else
        // {
        //     error(parser->cur_token.line, parser->cur_token.column, "Expecting '*' or type qualifier");
        // }
    }

    switch(parser->cur_token.type)
    {
        case TOKEN_IDENTIFIER:
            if(flags & PARSER_FLAG_TYPE_NAME)
            {
                error(parser->cur_token.line, parser->cur_token.column, "Unexpected identifier in type name");
            }            

            declarator->identifier = strndup(parser->cur_token.constant.string_constant, parser->cur_token.constant.string_length);
            advance_token(parser);
        break;

        case TOKEN_PUNCTUATOR:
            /* handle a parenthesized declarator or a parenthesized abstract-declarator */
            if(parser->cur_token.name == TOKEN_PUNCTUATOR_OPARENTHESIS)
            {
                if((flags & PARSER_FLAG_TYPE_NAME) && (parser->next_token.type != TOKEN_PUNCTUATOR ||
                    (parser->next_token.name != TOKEN_PUNCTUATOR_ASTERISC && parser->next_token.name != TOKEN_PUNCTUATOR_OPARENTHESIS)))
                {
                    /* a parenthesised abstract-declarator will start with a '*' or a '(', so if we get anything else
                    in here, it means we're dealing with a direct-abstract-declarator, so we gtfo here and let the
                    upcoming code handle it. */
                    break;
                }

                /* declarator is inside parenthesis, which means anything inside of it has precedence. If
                there are any '*', for example, the declarator will have pointer to something type */
                advance_token(parser);
                parse_declarator(parser, declarator, flags);

                if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                {
                    advance_token(parser);
                }
                else
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expecting ')' token");
                }
            }
        break;

        default:
            if(!(flags & (PARSER_FLAG_TYPE_NAME | PARSER_FLAG_ARG_LIST)))
            {
                /* a direct-declarator is supposed to have an identifier */
                error(parser->cur_token.line, parser->cur_token.column, "Expecting identifier or '(");
            }
        break;
    }
    

    /* handle direct-declarator that's not a parenthesized declarator or 
    direct-abstract-declarator that's not a parenthezised abstract-declarator */
    if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    {
        // struct type_t temp_type = {.type = TYPE_UNKNOWN};
        struct type_t *temp_type = pool_AddElement(&parser->types, NULL);
        temp_type->type = TYPE_UNKNOWN;

        /* this declarator may be a function or an array... */
        switch(parser->cur_token.name)
        {
            case TOKEN_PUNCTUATOR_OPARENTHESIS:
                temp_type->type = TYPE_FUNCTION;
                /* ( */
                advance_token(parser);

                /* this is the parameter list of a function, which means we'll be
                possibly declaring other identifiers, so we call the declaration
                parsing routine here...*/
                //parser->param_list_level++;

                // function_type->args = 0;
                temp_type->func.arg_count = 0;

                struct declarator_t *args = NULL;
                struct declarator_t *last_arg = NULL;

                if(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CPARENTHESIS)
                {
                    if(parser->cur_token.type == TOKEN_KEYWORD && parser->cur_token.name == TOKEN_KEYWORD_VOID &&
                        parser->next_token.type == TOKEN_PUNCTUATOR && parser->next_token.name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                    {
                        /* special case of a function that takes no parameters */

                        /* void */
                        advance_token(parser);
                    }
                    else
                    {
                        while(1)
                        {
                            /* parse parameter-list... */
                            struct declarator_t *arg = parse_declaration(parser, PARSER_FLAG_ARG_LIST);

                            if(arg)
                            {
                                temp_type->func.arg_count++;

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

                            if(parser->cur_token.type == TOKEN_PUNCTUATOR)
                            {
                                if(parser->cur_token.name == TOKEN_PUNCTUATOR_CPARENTHESIS)
                                {
                                    break;
                                }
                                else if(parser->cur_token.name == TOKEN_PUNCTUATOR_COMMA)
                                {
                                    advance_token(parser);

                                    if(!is_declaration_specifier(&parser->cur_token))
                                    {
                                        /* function parameters require declaration specifiers */
                                        error(parser->cur_token.line, parser->cur_token.column, "Expecting declaration specifiers");
                                    }
                                }
                                else
                                {
                                    /* error: unexpected token. Expecting ')' or ','... */
                                    error(parser->cur_token.line, parser->cur_token.column, "Unexpected token. Expecting ')' or','");
                                }
                            }
                        }
                    }
                }

                temp_type->func.args = args;

                /* ) */
                advance_token(parser);
            break;

            case TOKEN_PUNCTUATOR_OBRACKET:
                temp_type->type = TYPE_ARRAY;

                /* [... */
                advance_token(parser);

                if(parser->cur_token.type != TOKEN_PUNCTUATOR)
                {
                    // if(parser->cur_token.type == TOKEN_KEYWORD && parser->cur_token.name == TOKEN_KEYWORD_STATIC)
                    // {
                    //     advance_token(parser);
                    // }

                    // while(is_type_qualifier(&parser->cur_token))
                    // {
                    //     advance_token(parser);
                    // }

                    struct exp_node_t *assign_exp = parse_assignment_exp(parser);
                }

                if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_CBRACKET)
                {
                    /* ']' */
                    advance_token(parser);
                }
                else
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expected ']'");
                }
            break;

            // case TOKEN_PUNCTUATOR_CPARENTHESIS:
            //     /* end of function parameter list, end of recursive declarator or end of type name  */
            // break;
        }

        if(temp_type->type != TYPE_UNKNOWN)
        {
            temp_type->next = type;
            type = temp_type;
        }
        else
        {
            pool_RemoveElement(&parser->types, temp_type->element_index);
        }

        if(declarator->type == NULL)
        {
            if(type == NULL && declarator->identifier == NULL && !(flags & (PARSER_FLAG_TYPE_NAME | PARSER_FLAG_ARG_LIST)))
            {
                /* if the declarator has no type and no identifier, and we're not handling function parameters or
                type names (which doesn't need an identifier), we get rid of the declarator here, because this is likely
                a null expression */
                pool_RemoveElement(&parser->declarators, declarator->element_index);
                declarator = NULL;
            }
            else
            {
                declarator->type = type;
            }
        }
        else
        {
            struct type_t *declarator_type = declarator->type;

            while(declarator_type->next)
            {
                declarator_type = declarator_type->next;
            }       

            declarator_type->next = type;
        }
    }
    else
    {
        /* anything else is wrong... */
    }

    return declarator;
}

struct type_t *parse_initializer(struct parser_t *parser)
{

}

struct type_t *parse_aggregate_declaration(struct parser_t *parser)
{
    // struct type_t *type = calloc(sizeof(struct type_t), 1);
    struct type_t *type = NULL;
    uint32_t aggregate_type = type_specifier_from_token(&parser->cur_token);
    uint32_t type_exists = 0;
    // type->type = type_specifier_from_token(&parser->cur_token);
    // type->complete = 0;
    /* struct/union */
    advance_token(parser);

    if(parser->cur_token.type != TOKEN_IDENTIFIER && parser->cur_token.type != TOKEN_PUNCTUATOR)
    {
        error(parser->cur_token.line, parser->cur_token.column, "Expecting tag or '{'");
    }

    /* optional tag */
    if(parser->cur_token.type == TOKEN_IDENTIFIER)
    {
        strncpy(identifier, parser->cur_token.constant.string_constant, parser->cur_token.constant.string_length);
        identifier[parser->cur_token.constant.string_length] = '\0';

        type = parser->aggregate_types;
        while(type != NULL)
        {
            if(!strcmp(type->aggregate.identifier, identifier))
            {
                break;
            }
            type = type->aggregate.stored_next;
        }

        if(type == NULL)
        {
            type = pool_AddElement(&parser->types, NULL);
            type->complete = 0;
            type->aggregate.identifier = strdup(identifier);
            type->aggregate.stored_next = parser->aggregate_types;
            parser->aggregate_types = type;
        }

        advance_token(parser);
    }
    else
    {
        type = pool_AddElement(&parser->types, NULL);
        type->complete = 0;
    }

    type->type = TYPE_AGGREGATE;
    type->aggregate.type = aggregate_type;

    if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_OBRACE)
    {
        /* struct/union (identifier) { ... */
        advance_token(parser);

        struct declarator_t *fields = NULL;
        struct declarator_t *last_field = NULL;

        if(type->complete)
        {
            error(parser->cur_token.line, parser->cur_token.column, "Redefinition of aggregate type %s", identifier);
        }
        
        do
        {
            /* parse the fields, which can be structs as well */
            struct declarator_t *field = parse_declaration(parser, 0);

            if(field != NULL)
            {
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
            }
            else if(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CBRACE)
            {
                error(parser->cur_token.line, parser->cur_token.column, "Expecting '}'");
            }
        }
        while(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CBRACE);

        /* '}' */
        advance_token(parser);

        type->aggregate.fields = fields;
        type->complete = 1;
    }

    // if(type->aggregate.identifier != NULL)
    // {
    //     /* this aggregate type has a tag, so we stash it... */
    //     stash_aggregate_type(parser, type);
    // }

    return type;
}

void parse_statement(struct parser_t *parser)
{
    struct token_t *token;

    if(parser->cur_scope == NULL)
    {
        parse_compound_statement(parser);
    }
    else if(parser->cur_token.type == TOKEN_IDENTIFIER && parser->next_token.type == TOKEN_PUNCTUATOR && 
            parser->next_token.type == TOKEN_PUNCTUATOR_COLON)
    {
        parse_labeled_statement(parser);
    }
    else if(parser->cur_token.type == TOKEN_KEYWORD)
    {
        switch(parser->cur_token.name)
        {
            case TOKEN_KEYWORD_CASE:
            case TOKEN_KEYWORD_DEFAULT:
                parse_labeled_statement(parser);
            break;
        }
    }
    else if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_OBRACE)
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
    switch(parser->cur_token.type)
    {
        case TOKEN_IDENTIFIER:
        {
            advance_token(parser);
            if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_COLON)
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
            switch(parser->cur_token.name)
            {
                case TOKEN_KEYWORD_CASE:
                    advance_token(parser);
                    if(parser->cur_token.type == TOKEN_CONSTANT)
                    {
                        advance_token(parser);

                        if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_COLON)
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

                    if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_COLON)
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

    if(parser->cur_function != NULL)
    {
        parser->cur_function->type->func.body = parser->cur_scope;
        parser->cur_scope->function = parser->cur_function;
    }

    /* {... */
    advance_token(parser);

    while((parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CBRACE) && 
            parser->cur_token.type != TOKEN_EOF)
    {
        if(is_declaration_specifier(&parser->cur_token))
        {
            struct declarator_t *declarator = parse_declaration(parser, 0);

            if(declarator != NULL)
            {
                if(declarator->type->type == TYPE_FUNCTION)
                {
                    if(parser->cur_scope->parent != NULL)
                    {
                        error(parser->cur_token.line, parser->cur_token.column, "Functions may be declared only in file scope");
                    }

                    if(parser->cur_token.type == TOKEN_PUNCTUATOR)
                    {
                        if(parser->cur_token.name == TOKEN_PUNCTUATOR_OBRACE)
                        {
                            /* function body */
                            parser->cur_function = declarator;
                            parse_compound_statement(parser);
                            parser->cur_function = NULL;
                            declarator->type->complete = 1;
                        }
                        // else if(token->)
                        // {
                        //     error(parser->cur_token.line, parser->cur_token.column, "Expecting '{' token");
                        // }
                    }
                }

                // while(declarator)
                // {
                //     create_object(parser, declarator);
                //     declarator = declarator->next; 
                // }
            }
        }
        else
        {
            parse_statement(parser);
        }
    }

    if(parser->cur_scope->parent != NULL)
    {
        pop_scope(parser);

        if(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CBRACE)
        {
            error(parser->cur_token.line, parser->cur_token.column, "Expecting '}");
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
    struct exp_tree_t *exp_tree = NULL;
    struct exp_node_t *root = parse_comma_exp(parser);

    if(root != NULL)
    {
        exp_tree = pool_AddElement(&parser->exp_trees, NULL);
        exp_tree->root = root;
        exp_tree->next = NULL;
    }
    // exp_tree = parse_exp(parser);

    if(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_SEMICOLON)
    {
        error(parser->cur_token.line, parser->cur_token.column, "Expecting ';'");
    }

    /* ';' */
    advance_token(parser);

    if(exp_tree != NULL)
    {
        if(parser->cur_scope->expressions == NULL)
        {
            parser->cur_scope->expressions = exp_tree;
        }
        else
        {
            parser->cur_scope->last_expression = exp_tree;
        }

        parser->cur_scope->last_expression = exp_tree;
    }
}


struct type_t int_type = {
    .type = TYPE_PRIMITIVE,
    .primitive.type = PRIM_TYPE_INT
};

struct type_t float_type = {
    .type = TYPE_PRIMITIVE,
    .primitive.type = PRIM_TYPE_FLOAT
};

struct type_t literal_type = {
    .type = TYPE_POINTER,
    .next = &(struct type_t){
        .type = TYPE_PRIMITIVE,
        .primitive.type = PRIM_TYPE_CHAR
    }
};

// struct type_t literal_type = {
//     .type = TYPE_PRIMITIVE,
//     .pointer = &(struct pointer_t){.count = 1},
//     .primitive.type = PRIM_TYPE_CHAR
// };

// struct exp_tree_t *parse_exp(struct parser_t *parser)
// {
//     struct exp_tree_t *tree = NULL;
//     struct exp_node_t *node = comma_exp(parser);

//     if(node != NULL)
//     {
//         tree = calloc(1, sizeof(struct exp_tree_t));
//         tree->root = node;
//     }

//     return tree;
// }

struct exp_node_t *parse_comma_exp(struct parser_t *parser)
{
    return parse_assignment_exp(parser);
}

uint32_t is_assignment_token(struct token_t *token)
{
    if(token->type == TOKEN_PUNCTUATOR)
    {
        switch(token->name)
        {
            case TOKEN_PUNCTUATOR_ASSIGN:
            case TOKEN_PUNCTUATOR_PLUS_ASSIGN:
            case TOKEN_PUNCTUATOR_MINUS_ASSIGN:
            case TOKEN_PUNCTUATOR_MUL_ASSIGN:
            case TOKEN_PUNCTUATOR_DIV_ASSIGN:
            case TOKEN_PUNCTUATOR_MOD_ASSIGN:
            case TOKEN_PUNCTUATOR_BW_AND_ASSIGN:
            case TOKEN_PUNCTUATOR_BW_OR_ASSIGN:
            case TOKEN_PUNCTUATOR_BW_XOR_ASSIGN:
                return 1;
            break;
        }
    }

    return 0;
}

uint32_t assign_exp_node_type_lut[] = {
    [TOKEN_PUNCTUATOR_ASSIGN]           = EXP_NODE_TYPE_ASSIGN,
    [TOKEN_PUNCTUATOR_PLUS_ASSIGN]      = EXP_NODE_TYPE_ADD_ASSIGN,
    [TOKEN_PUNCTUATOR_MINUS_ASSIGN]     = EXP_NODE_TYPE_SUB_ASSIGN,
    [TOKEN_PUNCTUATOR_MUL_ASSIGN]       = EXP_NODE_TYPE_MUL_ASSIGN,
    [TOKEN_PUNCTUATOR_DIV_ASSIGN]       = EXP_NODE_TYPE_DIV_ASSIGN,
    [TOKEN_PUNCTUATOR_MOD_ASSIGN]       = EXP_NODE_TYPE_MOD_ASSIGN,
    [TOKEN_PUNCTUATOR_BW_AND_ASSIGN]    = EXP_NODE_TYPE_AND_ASSIGN,
    [TOKEN_PUNCTUATOR_BW_OR_ASSIGN]     = EXP_NODE_TYPE_OR_ASSIGN,
    [TOKEN_PUNCTUATOR_BW_XOR_ASSIGN]    = EXP_NODE_TYPE_XOR_ASSIGN,
};

struct exp_node_t *parse_assignment_exp(struct parser_t *parser)
{
    /* I'm not entirely sure about this. The assignment-expression can produce either
    "conditional expression" or a "unary-expression op assignment-expression". It'd seem
    like backtracking would be necessary to choose which production rule to use, but I
    guess checking if the result of this call is an lvalue is enough to choose the proper
    rule. I guess this is fine, since "conditional-expression" can also derive "unary-expression". */
    struct exp_node_t *left_exp = parse_conditional_exp(parser);    

    if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    {
        if(is_assignment_token(&parser->cur_token))
        {
            if(!left_exp->lvalue)
            {
                error(parser->cur_token.line, parser->cur_token.column, "Can't assign to a non-lvalue");
            }

            struct exp_node_t *assign_exp = pool_AddElement(&parser->exp_nodes, NULL);
            assign_exp->type = assign_exp_node_type_lut[parser->cur_token.name];

            /* '=', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=' */
            advance_token(parser);
            
            assign_exp->left = left_exp;
            assign_exp->right = parse_assignment_exp(parser);
            assign_exp->lvalue = 0;
            assign_exp->value = left_exp->value;
            left_exp = assign_exp;
        }
    }

    return left_exp;
}

struct exp_node_t *parse_conditional_exp(struct parser_t *parser)
{
    return parse_logical_or_exp(parser);
}

struct exp_node_t *parse_logical_or_exp(struct parser_t *parser)
{
    return parse_logical_and_exp(parser);
}

struct exp_node_t *parse_logical_and_exp(struct parser_t *parser)
{
    return parse_or_exp(parser);
}

struct exp_node_t *parse_or_exp(struct parser_t *parser)
{
    return parse_xor_exp(parser);
}

struct exp_node_t *parse_xor_exp(struct parser_t *parser)
{
    return parse_and_exp(parser);
}

struct exp_node_t *parse_and_exp(struct parser_t *parser)
{
    return parse_equality_exp(parser);
}

struct exp_node_t *parse_equality_exp(struct parser_t *parser)
{
    return parse_relational_exp(parser);
}

struct exp_node_t *parse_relational_exp(struct parser_t *parser)
{
    return parse_shift_exp(parser);
}

uint32_t shift_exp_node_type_lut[] = {
    [TOKEN_PUNCTUATOR_LEFT_SHIFT] = EXP_NODE_TYPE_SHIFT_LEFT,
    [TOKEN_PUNCTUATOR_RIGHT_SHIFT] = EXP_NODE_TYPE_SHIFT_RIGHT,
};

struct exp_node_t *parse_shift_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp;
    left_exp = parse_additive_exp(parser);

    // if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    // {
    //     switch(parser->cur_token.name)
    //     {
    //         case TOKEN_PUNCTUATOR_LEFT_SHIFT:
    //         case TOKEN_PUNCTUATOR_RIGHT_SHIFT:
    //         {
    //             struct exp_node_t *shift_exp = calloc(1, sizeof(struct exp_node_t));
    //             shift_exp->type = EXP_NODE_TYPE_SHIFT;
    //             shift_exp->sub_type = shift_exp_node_type_lut[parser->cur_token.name];
    //             advance_token(parser);
    //             shift_exp->left = left_exp;
    //             shift_exp->right = additive_exp(parser);
    //             left_exp = shift_exp;        
    //         }
    //         break;
    //     }
    // }

    return left_exp;
}


uint32_t additive_exp_node_type_lut[] = {
    [TOKEN_PUNCTUATOR_PLUS] = EXP_NODE_TYPE_ADD,
    [TOKEN_PUNCTUATOR_MINUS] = EXP_NODE_TYPE_SUB,
};

struct exp_node_t *parse_additive_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp;
    left_exp = parse_multiplicative_exp(parser);
    // token = parser->cur_token;

    // if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    // {
    //     switch(parser->cur_token.name)
    //     {
    //         case TOKEN_PUNCTUATOR_PLUS:
    //         case TOKEN_PUNCTUATOR_MINUS:
    //         {
    //             struct exp_node_t *add_exp = calloc(1, sizeof(struct exp_node_t));
    //             add_exp->type = EXP_NODE_TYPE_ADDITIVE;
    //             add_exp->sub_type = additive_exp_node_type_lut[parser->cur_token.name];
    //             advance_token(parser);
    //             add_exp->left = left_exp;
    //             add_exp->right = multiplicative_exp(parser);
    //             left_exp = add_exp;
    //         }
    //         break;
    //         //     node_type = ADDITIVE_EXP_NODE_TYPE_ADD;
    //         // break;

    //         // case TOKEN_PUNCTUATOR_MINUS:
    //         //     node_type = ADDITIVE_EXP_NODE_TYPE_SUB;
    //         // break;

    //         // default:
    //         //     has_add = 0;
    //         // break;
    //     }
    // }

//     if(has_add)
//     {
//         add_exp = calloc(1, sizeof(struct exp_node_t));
//         add_exp->left = left_exp;
//         add_exp->right = multiplicative_exp(parser);
//         add_exp->type = EXP_NODE_TYPE_ADDITIVE;
//         add_exp->sub_type = node_type;
//         left_exp = add_exp;
//    }

    return left_exp;
}

uint32_t multiplicative_exp_node_type_lut[] = {
    [TOKEN_PUNCTUATOR_ASTERISC]     = EXP_NODE_TYPE_MUL,
    [TOKEN_PUNCTUATOR_SLASH]        = EXP_NODE_TYPE_DIV,
    [TOKEN_PUNCTUATOR_PERCENT]      = EXP_NODE_TYPE_MOD,
};

struct exp_node_t *parse_multiplicative_exp(struct parser_t *parser)
{
    // struct exp_node_t *mul_exp;
    struct exp_node_t *left_exp;
    // struct token_t *token;
    // uint32_t node_type;
    // uint32_t has_mult = 0;

    left_exp = parse_cast_exp(parser);
    // token = parser->cur_token;

    // if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    // {
    //     // has_mult = 1;
    //     switch(parser->cur_token.name)
    //     {
    //         case TOKEN_PUNCTUATOR_ASTERISC:
    //         case TOKEN_PUNCTUATOR_SLASH:
    //         case TOKEN_PUNCTUATOR_PERCENT:
    //         {
    //             struct exp_node_t *mul_exp = calloc(1, sizeof(struct exp_node_t));
    //             mul_exp->type = EXP_NODE_TYPE_MULTIPLICATIVE;
    //             mul_exp->sub_type = multiplicative_exp_node_type_lut[parser->cur_token.name];
    //             advance_token(parser);
    //             mul_exp->left = left_exp;
    //             mul_exp->right = cast_exp(parser);
    //             left_exp = mul_exp;
    //         }
    //         break;

    //         // case TOKEN_PUNCTUATOR_SLASH:
    //         //     node_type = MULTIPLICATIVE_EXP_NODE_TYPE_DIV;
    //         // break;

    //         // case TOKEN_PUNCTUATOR_PERCENT:
    //         //     node_type = MULTIPLICATIVE_EXP_NODE_TYPE_MOD;
    //         // break;

    //         // default:
    //         //     has_mult = 0;
    //         // break;
    //     }
    // }

    // if(has_mult)
    // {
    //     advance_token(parser);
    //     // mul_exp = (struct multiplicative_exp_node_t *)new_node(EXP_NODE_TYPE_MULTIPLICATIVE);
    //     mul_exp = calloc(1, sizeof(struct exp_node_t));
    //     mul_exp->type = EXP_NODE_TYPE_MULTIPLICATIVE;
    //     mul_exp->sub_type = node_type;
    //     mul_exp->left = left_exp;
    //     mul_exp->right = cast_exp(parser);
    //     left_exp = mul_exp;
    // }

    return left_exp;
}

struct exp_node_t *parse_cast_exp(struct parser_t *parser)
{
    struct exp_node_t *exp;

    if(parser->cur_token.type == TOKEN_PUNCTUATOR && parser->cur_token.name == TOKEN_PUNCTUATOR_OPARENTHESIS)
    {
        /* ( */
        advance_token(parser);
        struct declarator_t *type_name = parse_declaration(parser, PARSER_FLAG_TYPE_NAME);

        if(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CPARENTHESIS)
        {
            error(parser->cur_token.line, parser->cur_token.column, "Expecting ')' token");
        }

        advance_token(parser);
        exp = parse_cast_exp(parser);
        exp->lvalue = 0;

        if(exp == NULL)
        {
            error(parser->cur_token.line, parser->cur_token.column, "Expecting expression");
        }
    }
    else
    {
        exp = parse_unary_exp(parser);
    }

    return exp;
}


// uint32_t unary_exp_node_type_lut[] = {
//     [TOKEN_PUNCTUATOR_ASTERISC] = 
// };

uint32_t is_unary_token(struct token_t *token)
{
    if(token->type == TOKEN_PUNCTUATOR)
    {
        switch(token->name)
        {
            case TOKEN_PUNCTUATOR_INCREMENT:
            case TOKEN_PUNCTUATOR_DECREMENT:
            case TOKEN_PUNCTUATOR_AMPERSAND:
            case TOKEN_PUNCTUATOR_ASTERISC:
            case TOKEN_PUNCTUATOR_PLUS:
            case TOKEN_PUNCTUATOR_MINUS:
            case TOKEN_PUNCTUATOR_TILDE:
            case TOKEN_PUNCTUATOR_EXCLAMATION:
                return 1;
            break;
        }
    }
    else if(token->type == TOKEN_KEYWORD && token->name == TOKEN_KEYWORD_SIZEOF)
    {
        return 1;
    }

    return 0;
}

struct exp_node_t *parse_unary_exp(struct parser_t *parser)
{
    struct token_t *token;
    struct exp_node_t *un_exp = NULL;
    uint32_t node_type;
    uint32_t has_unary = 0;

    // token = parser->cur_token;

    // switch(parser->cur_token.type)
    // {
    //     case TOKEN_KEYWORD:
    //         has_unary = 1;
    //         switch(parser->cur_token.name)
    //         {
    //             case TOKEN_KEYWORD_SIZEOF:
    //                 node_type = UNARY_EXP_NODE_TYPE_SIZEOF;
    //             break;
    //         }
    //     break;

    //     case TOKEN_PUNCTUATOR:
    //         has_unary = 1;
    //         switch(parser->cur_token.name)
    //         {
    //             case TOKEN_PUNCTUATOR_ASTERISC:
    //                 node_type = UNARY_EXP_NODE_TYPE_DEREFERENCE;
    //             break;

    //             case TOKEN_PUNCTUATOR_PLUS:
    //                 node_type = UNARY_EXP_NODE_TYPE_PLUS;
    //             break;

    //             case TOKEN_PUNCTUATOR_MINUS:
    //                 node_type = UNARY_EXP_NODE_TYPE_MINUS;
    //             break;

    //             case TOKEN_PUNCTUATOR_AMPERSAND:
    //                 node_type = UNARY_EXP_NODE_TYPE_ADDRESS_OF;
    //             break;

    //             case TOKEN_PUNCTUATOR_INCREMENT:
    //                 node_type = UNARY_EXP_NODE_TYPE_INCREMENT;
    //             break;

    //             case TOKEN_PUNCTUATOR_DECREMENT:
    //                 node_type = UNARY_EXP_NODE_TYPE_DECREMENT;
    //             break;

    //             case TOKEN_PUNCTUATOR_EXCLAMATION:
    //                 node_type = UNARY_EXP_NODE_TYPE_LOGICAL_NOT;
    //             break;

    //             case TOKEN_PUNCTUATOR_TILDE:
    //                 node_type = UNARY_EXP_NODE_TYPE_BITWISE_NOT;
    //             break;

    //             default:
    //                 has_unary = 0;
    //             break;
    //         }
    //     break;
    // } 

    // if(has_unary)
    // {
    //     advance_token(parser);
    //     un_exp = calloc(1, sizeof(struct exp_node_t));
    //     un_exp->type = EXP_NODE_TYPE_UNARY;
    //     un_exp->sub_type = node_type;
    //     un_exp->left = unary_exp(parser);
    // }
    // else
    {
        un_exp = parse_postfix_exp(parser);
    }

    return un_exp;
}

uint32_t is_postfix_token(struct token_t *token)
{
    if(token->type == TOKEN_PUNCTUATOR)
    {
        switch(token->name)
        {
            case TOKEN_PUNCTUATOR_OPARENTHESIS:
            case TOKEN_PUNCTUATOR_OBRACKET:
            case TOKEN_PUNCTUATOR_INCREMENT:
            case TOKEN_PUNCTUATOR_DECREMENT:
            case TOKEN_PUNCTUATOR_DOT:
            case TOKEN_PUNCTUATOR_ARROW:
                return 1;
            break;
        }
    }

    return 0;
}

uint32_t postfix_exp_node_type_lut[] = {
    [TOKEN_PUNCTUATOR_OPARENTHESIS]     = EXP_NODE_TYPE_FUNC_CALL,
    [TOKEN_PUNCTUATOR_OBRACKET]         = EXP_NODE_TYPE_ARRAY_INDEX,
    [TOKEN_PUNCTUATOR_INCREMENT]        = EXP_NODE_TYPE_POS_INCREMENT,
    [TOKEN_PUNCTUATOR_DECREMENT]        = EXP_NODE_TYPE_POS_DECREMENT,
    [TOKEN_PUNCTUATOR_DOT]              = EXP_NODE_TYPE_DOT,
    [TOKEN_PUNCTUATOR_ARROW]            = EXP_NODE_TYPE_ARROW
};

struct exp_node_t *parse_postfix_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp;

    left_exp = parse_primary_exp(parser);

    // if(is_postfix_token(&parser->cur_token))
    while(is_postfix_token(&parser->cur_token))
    {
        struct exp_node_t *pos_exp = pool_AddElement(&parser->exp_nodes, NULL);
        pos_exp->left = NULL;
        pos_exp->right = NULL;
        pos_exp->type = postfix_exp_node_type_lut[parser->cur_token.name];
        advance_token(parser);

        switch(pos_exp->type)
        {
            case EXP_NODE_TYPE_ARRAY_INDEX:
            {
                struct type_t *pointed_type = left_exp->effective_type->next;

                if(left_exp->effective_type->type == TYPE_POINTER)
                {
                    if(!pointed_type->complete)
                    {
                        error(parser->cur_token.line, parser->cur_token.column, "Can't index array of incomplete type");
                    }
                }
                else
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Type is not indexable");
                }

                struct exp_node_t *exp = parse_comma_exp(parser);

                if(parser->cur_token.type != TOKEN_PUNCTUATOR || parser->cur_token.name != TOKEN_PUNCTUATOR_CBRACKET)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expected ']'");
                }

                if(exp == NULL)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expected expression");
                }

                if(exp->effective_type->type != TYPE_PRIMITIVE || (exp->effective_type->primitive.type != PRIM_TYPE_INT &&
                                                                   exp->effective_type->primitive.type != PRIM_TYPE_UINT))
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Array index must be an integer");
                }

                /* ...] */
                advance_token(parser);

                pos_exp->left = left_exp;
                pos_exp->right = exp;
                pos_exp->effective_type = pointed_type;
                pos_exp->lvalue = 1;
            }
            break;

            case EXP_NODE_TYPE_DOT:
            {
                if(parser->cur_token.type != TOKEN_IDENTIFIER)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Expected identifier");
                }

                struct type_t *type = left_exp->effective_type;

                if(type->type != TYPE_AGGREGATE)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Type is not a struct nor union");
                }

                strncpy(identifier, parser->cur_token.constant.string_constant, parser->cur_token.constant.string_length);
                identifier[parser->cur_token.constant.string_length] = '\0';

                struct declarator_t *field = type->aggregate.fields;

                while(field)
                {
                    if(!strcmp(field->identifier, identifier))
                    {
                        break;
                    }
                    field = field->next;
                }

                if(field == NULL)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Aggregate type has no field named \"%s\"", identifier);
                }

                /* identifier */
                advance_token(parser);

                pos_exp->left = left_exp;
                pos_exp->effective_type = field->type;
                pos_exp->lvalue = 1;
            }
            break;
        }

        left_exp = pos_exp;
    }

    // if(parser->cur_token.type == TOKEN_PUNCTUATOR)
    // {
        // has_postfix = 1;
        // switch(parser->cur_token.name)
        // {
        //     // case TOKEN_PUNCTUATOR_OPARENTHESIS:
        //     //     node_type = POSTFIX_EXP_NODE_TYPE_FUNC_CALL;
        //     // break;

        //     case TOKEN_PUNCTUATOR_OBRACKET:
        //         node_type = POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX;
        //     break;

        //     // case TOKEN_PUNCTUATOR_INCREMENT:
        //     //     node_type = POSTFIX_EXP_NODE_TYPE_INCREMENT;
        //     // break;

        //     // case TOKEN_PUNCTUATOR_DECREMENT:
        //     //     node_type = POSTFIX_EXP_NODE_TYPE_DECREMENT;
        //     // break;

        //     // case TOKEN_PUNCTUATOR_DOT:
        //     //     node_type = POSTFIX_EXP_NODE_TYPE_DOT;
        //     // break;

        //     // case TOKEN_PUNCTUATOR_ARROW:
        //     //     node_type = POSTFIX_EXP_NODE_TYPE_ARROW;
        //     // break;

        //     // default:
        //     //     has_postfix = 0;
        //     // break;
        // }
    // }

    // if(has_postfix)
    // {
    //     advance_token(parser);
    //     pos_exp = calloc(1, sizeof(struct exp_node_t));
    //     pos_exp->type = EXP_NODE_TYPE_POSTFIX;
    //     pos_exp->sub_type = node_type;
    //     pos_exp->left = left_exp;
    //     if(node_type == POSTFIX_EXP_NODE_TYPE_FUNC_CALL || node_type == POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX)
    //     {
    //         pos_exp->right = comma_exp(parser);
    //         advance_token(parser);
    //     }
    //     else
    //     {
    //         pos_exp->right = postfix_exp(parser);
    //     }

    //     left_exp = pos_exp;
    // }

    return left_exp;
}

uint32_t is_primary_token(struct parser_t *parser)
{
    return parser->cur_token.type == TOKEN_IDENTIFIER ||
           parser->cur_token.type == TOKEN_CONSTANT ||
           parser->cur_token.type == TOKEN_STRING_LITERAL ||
           (parser->cur_token.type == TOKEN_PUNCTUATOR && 
            parser->cur_token.name == TOKEN_PUNCTUATOR_OPARENTHESIS);
}

struct exp_node_t *parse_primary_exp(struct parser_t *parser)
{
    struct exp_node_t *node = NULL;

    if(parser->cur_token.type != TOKEN_EOF)
    {
        switch(parser->cur_token.type)
        {
            case TOKEN_IDENTIFIER:
            {
                // node = calloc(1, sizeof(struct exp_node_t));
                node = pool_AddElement(&parser->exp_nodes, NULL);
                node->type = EXP_NODE_TYPE_IDENTIFIER;
                // node->sub_type = PRIMARY_EXP_NODE_TYPE_IDENTIFIER;
                strncpy(identifier, parser->cur_token.constant.string_constant, parser->cur_token.constant.string_length);
                identifier[parser->cur_token.constant.string_length] = '\0';
                node->value.object = get_object(parser, identifier);
                node->lvalue = 1;

                if(node->value.object == NULL)
                {
                    error(parser->cur_token.line, parser->cur_token.column, "Undeclared indentifier '%s'", identifier);
                }

                struct type_t *type = node->value.object->declarator->type;

                if(type->type == TYPE_ARRAY)
                {
                    /* arrays decay into pointers */
                    struct type_t *pointer_type = pool_AddElement(&parser->types, NULL);
                    pointer_type->type = TYPE_POINTER;
                    pointer_type->next = type->next;
                    type = pointer_type;
                }

                node->effective_type = type;
                advance_token(parser);
            }
            break;

            case TOKEN_CONSTANT:
                node = pool_AddElement(&parser->exp_nodes, NULL);
                node->type = EXP_NODE_TYPE_CONSTANT;
                node->lvalue = 0;
                // node->effective_type = calloc(1, sizeof(struct type_t));
                // node->sub_type = PRIMARY_EXP_NODE_TYPE_CONSTANT;
                switch(parser->cur_token.name)
                {
                    case TOKEN_CONSTANT_INTEGER:
                        node->value.constant.int_constant = parser->cur_token.constant.int_constant;
                        node->effective_type = &int_type;
                        // node->effective_type->type = TYPE_PRIMITIVE;
                        // node->effective_type->primitive.type = PRIM_TYPE_INT;
                    break;

                    case TOKEN_CONSTANT_FLOAT:
                        node->value.constant.float_constant = parser->cur_token.constant.float_constant;
                        node->effective_type = &float_type;
                        // node->effective_type->type = TYPE_PRIMITIVE;
                        // node->effective_type->primitive.type = PRIM_TYPE_FLOAT;
                    break;
                }
                advance_token(parser);
            break;

            case TOKEN_STRING_LITERAL:
                node = pool_AddElement(&parser->exp_nodes, NULL);
                node->type = EXP_NODE_TYPE_LITERAL;
                node->lvalue = 1;
                // node->sub_type = PRIMARY_EXP_NODE_TYPE_STRING_LITERAL;
                node->value.constant.string_constant = strndup(parser->cur_token.constant.string_constant, parser->cur_token.constant.string_length);
                node->effective_type = &literal_type;
                // node->effective_type = calloc(1, sizeof(struct type_t));
                // node->effective_type->
                advance_token(parser);
            break;

            case TOKEN_PUNCTUATOR:
                if(parser->cur_token.name == TOKEN_PUNCTUATOR_OPARENTHESIS)
                {
                    // node = calloc(1, sizeof(struct exp_node_t));
                    // node->sub_type = PRIMARY_EXP_NODE_TYPE_EXPRESSION;
                    /* (... */
                    advance_token(parser);

                    node = parse_comma_exp(parser);
                    /* ...) */
                    advance_token(parser);
                }
            break;
        }

        // if(node != NULL)
        // {
        //     node->type = EXP_NODE_TYPE_PRIMARY;
        // }
    }

    return node;
}



void dump_scope(struct parser_t *parser, struct scope_t *scope, uint32_t depth)
{
    if(scope->function != NULL)
    {
        for(uint32_t index = 0; index < depth; index++)
        {
            putchar(' ');
        }
        if(scope->function->type->func.args != NULL)
        {
            printf("function (%s)(", scope->function->identifier);

            struct declarator_t *arg = scope->function->type->func.args;
            while(arg)
            {
                if(arg->next != NULL)
                {
                    printf("(%s), ", arg->identifier);
                }
                else
                {
                    printf("(%s)", arg->identifier);
                }
                arg = arg->next;
            }

            printf(")\n");
        }
        else
        {
            printf("function (%s)(no args)\n", scope->function->identifier);
        }
    }
    else if(scope->parent == NULL)
    {
        printf("file\n");
    }

    for(uint32_t index = 0; index < depth; index++)
    {
        putchar(' ');
    }

    printf("{\n");

    struct object_t *object = scope->objects;

    while(object)
    {
        if(object->declarator->type->type != TYPE_FUNCTION)
        {
            for(uint32_t index = 0; index < depth + 1; index++)
            {
                putchar(' ');
            }
            
            printf("object (%s)\n", object->declarator->identifier);
        }
           
        object = object->next;
    }

    struct scope_t *child_scope = scope->children;
    while(child_scope)
    {
        dump_scope(parser, child_scope, depth + 1);
        child_scope = child_scope->next;
    }

    for(uint32_t index = 0; index < depth; index++)
    {
        putchar(' ');
    }

    printf("}\n");
}

void dump_program(struct parser_t *parser)
{
    dump_scope(parser, parser->cur_scope, 0);
}




