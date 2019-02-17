#include "parse.h"
#include "type.h"

#include <stdio.h>
#include <stdlib.h>




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


void translate_type(struct base_type_t *type, int verbose)
{
    struct identifier_type_t *identifier;
    struct function_type_t *function;
    struct base_type_t *params;

    while(type)
    {
        switch(type->type)
        {
            case TYPE_IDENTIFIER:
                identifier = (struct identifier_type_t *)type;
                if(verbose)
                {
                    printf("[%s] is ", identifier->identifier);
                }
                else
                {
                    printf("[%s] ", identifier->identifier);
                }

            break;

            case TYPE_POINTER:

                if(verbose)
                {
                    printf("a pointer to ");
                }
                else
                {
                    printf("* ");
                }

            break;

            case TYPE_FUNCTION:
                function = (struct function_type_t *)type;

                printf("a function (");

                params = function->params;

                while(params)
                {
                    translate_type(params, 1);

                    while(params && params->type != TYPE_UNKNOWN)
                    {
                        params = params->next;
                    }

                    params = params->next;

                    if(params)
                    {
                        printf(", ");
                    }
                }

                printf("), returning ");
            break;

            case TYPE_INT:
                if(verbose)
                {
                    printf("an int ");
                }
                else
                {
                    printf("int");
                }

            break;

            case TYPE_VOID:
                printf("void ");
            break;

            case TYPE_SHORT:
                if(verbose)
                {
                    printf("a short ");
                }
                else
                {
                    printf("short");
                }

            break;

            case TYPE_CHAR:
                printf("a char ");
            break;

            default:
                return;
            break;
        }

        type = type->next;
    }

    printf("\n");
}

#define DECL_MAX_DEPTH 8192

void parse_tokens(struct token_t *tokens)
{
    struct token_t *token;
    struct parser_t parser;

    //parser.state = PARSER_STATE_NONE;

    parser.tokens = tokens;
    parser.current_token = tokens;
    parser.prev_token = NULL;

    parser.param_list_level = 0;

    while(parser.current_token->token_type != TOKEN_EOF)
    {
        if(is_type_specifier(parser.current_token))
        {
            translate_type(parse_declaration(&parser), 1);
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

    struct base_type_t *specifiers_qualifiers = NULL;
    struct base_type_t *last_specifier_qualifier = NULL;

    struct base_type_t *new_type = NULL;
    struct base_type_t *current_type = NULL;
    struct base_type_t *prev_type = NULL;
    struct base_type_t *declarator = NULL;
    struct base_type_t *temp = NULL;

    int type_type;

    if(is_type_specifier(parser->current_token))
    {
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
            else
            {
                if(is_type_specifier(parser->current_token) && specifiers_qualifiers)
                {
                    /* error... */
                }
            }

            while(is_type_specifier(parser->current_token))
            {
                /* type specifiers: int, short, unsigned, etc... */
                type_type = type_from_token(parser->current_token);

                new_type = calloc(sizeof(struct base_type_t), 1);

                new_type->next = NULL;
                new_type->type = type_type;

                if(!specifiers_qualifiers)
                {
                    specifiers_qualifiers = new_type;
                }
                else
                {
                    last_specifier_qualifier->next = type;
                }

                last_specifier_qualifier = new_type;

                advance_token(parser);
            }

            current_type = specifiers_qualifiers;

            /* declarator: identifier, function, array, pointer, etc... */
            declarator = parse_declarator(parser);

            if(declarator)
            {
                temp = declarator;

                while(temp->next)
                {
                    temp = temp->next;
                }

                temp->next = specifiers_qualifiers;

                current_type = declarator;
            }

            temp = current_type;

            while(temp->next)
            {
                temp = temp->next;
            }

            /* unknown type will be used to mark the end
            of a type... */
            new_type = calloc(sizeof(struct base_type_t), 1);
            new_type->next = NULL;
            new_type->type = TYPE_UNKNOWN;

            temp->next = new_type;

            if(!type)
            {
                type = current_type;
            }
            else
            {
                temp = prev_type;

                while(temp->next)
                {
                    temp = temp->next;
                }

                temp->next = current_type;
            }

            prev_type = current_type;

            if(parser->current_token->token_type != TOKEN_PUNCTUATOR ||
               parser->current_token->token_name != TOKEN_PUNCTUATOR_COMMA)
            {
                break;
            }

            advance_token(parser);

        }while(1);
    }

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


                parser->param_list_level++;

                new_type = parse_declaration(parser);

                parser->param_list_level--;

                if(!new_type)
                {
                    /* empty params, implies void
                    param... */
                    new_type = calloc(sizeof(struct base_type_t), 1);
                    new_type->next = NULL;
                    new_type->type = TYPE_VOID;
                }


                function_type->params = new_type;

                /* ) */
                advance_token(parser);

            break;

            case TOKEN_PUNCTUATOR_OBRACKET:

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





