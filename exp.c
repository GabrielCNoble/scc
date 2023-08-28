#include "exp.h"
#include "parse.h"
#include "obj.h"
// struct exp_node_t *new_node(uint32_t type)
// {
//     uint32_t size = sizeof(struct base_exp_node_t);
//     struct base_exp_node_t *node;

//     switch(type)
//     {
//         case EXP_NODE_TYPE_PRIMARY:
//             size = sizeof(struct primary_exp_node_t);
//         break;

//         case EXP_NODE_TYPE_POSTFIX:
//             size = sizeof(struct postfix_exp_node_t);
//         break;

//         case EXP_NODE_TYPE_UNARY:
//             size = sizeof(struct unary_exp_node_t);
//         break;

//         case EXP_NODE_TYPE_MULTIPLICATIVE:
//             size = sizeof(struct multiplicative_exp_node_t);
//         break;

//         case EXP_NODE_TYPE_ADDITIVE:
//             size = sizeof(struct additive_exp_node_t);
//         break;
//     }

//     node = calloc(1, size);
//     node->type = type;

//     return node;
// }

static char identifier[8192];

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

struct exp_tree_t *parse_exp(struct parser_t *parser)
{
    struct exp_tree_t *tree = NULL;
    struct exp_node_t *node = comma_exp(parser);

    if(node != NULL)
    {
        tree = calloc(1, sizeof(struct exp_tree_t));
        tree->root = node;
    }

    return tree;
}

struct exp_node_t *comma_exp(struct parser_t *parser)
{
    return assignment_exp(parser);
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

struct exp_node_t *assignment_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp = conditional_exp(parser);    

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

            advance_token(parser);
            assign_exp->left = left_exp;
            assign_exp->right = assignment_exp(parser);
            assign_exp->lvalue = 0;
            assign_exp->value = left_exp->value;
            left_exp = assign_exp;
        }
    }

    return left_exp;
}

struct exp_node_t *conditional_exp(struct parser_t *parser)
{
    return logical_or_exp(parser);
}

struct exp_node_t *logical_or_exp(struct parser_t *parser)
{
    return logical_and_exp(parser);
}

struct exp_node_t *logical_and_exp(struct parser_t *parser)
{
    return or_exp(parser);
}

struct exp_node_t *or_exp(struct parser_t *parser)
{
    return xor_exp(parser);
}

struct exp_node_t *xor_exp(struct parser_t *parser)
{
    return and_exp(parser);
}

struct exp_node_t *and_exp(struct parser_t *parser)
{
    return equality_exp(parser);
}

struct exp_node_t *equality_exp(struct parser_t *parser)
{
    return relational_exp(parser);
}

struct exp_node_t *relational_exp(struct parser_t *parser)
{
    return shift_exp(parser);
}

uint32_t shift_exp_node_type_lut[] = {
    [TOKEN_PUNCTUATOR_LEFT_SHIFT] = EXP_NODE_TYPE_SHIFT_LEFT,
    [TOKEN_PUNCTUATOR_RIGHT_SHIFT] = EXP_NODE_TYPE_SHIFT_RIGHT,
};

struct exp_node_t *shift_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp;
    left_exp = additive_exp(parser);

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

struct exp_node_t *additive_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp;
    left_exp = multiplicative_exp(parser);
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

struct exp_node_t *multiplicative_exp(struct parser_t *parser)
{
    // struct exp_node_t *mul_exp;
    struct exp_node_t *left_exp;
    // struct token_t *token;
    // uint32_t node_type;
    // uint32_t has_mult = 0;

    left_exp = cast_exp(parser);
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

struct exp_node_t *cast_exp(struct parser_t *parser)
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
        exp = cast_exp(parser);
        exp->lvalue = 0;

        if(exp == NULL)
        {
            error(parser->cur_token.line, parser->cur_token.column, "Expecting expression");
        }
    }
    else
    {
        exp = unary_exp(parser);
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

struct exp_node_t *unary_exp(struct parser_t *parser)
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
        un_exp = postfix_exp(parser);
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

struct exp_node_t *postfix_exp(struct parser_t *parser)
{
    struct exp_node_t *left_exp;

    left_exp = primary_exp(parser);

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

                struct exp_node_t *exp = comma_exp(parser);

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

struct exp_node_t *primary_exp(struct parser_t *parser)
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

                    node = comma_exp(parser);
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

void traverse_exp_tree(struct exp_node_t *exp)
{
    // struct primary_exp_node_t *prim_exp;
    // struct postfix_exp_node_t *post_exp;
    // struct unary_exp_node_t *un_exp;
    // struct multiplicative_exp_node_t *mul_exp;
    // struct additive_exp_node_t *add_exp;

    if(!exp)
    {
        return;
    }

    // switch(exp->type)
    // {
    //     case EXP_NODE_TYPE_PRIMARY:
    //         // prim_exp = (struct primary_exp_node_t *)exp;
    //         switch(exp->sub_type)
    //         {
    //             case PRIMARY_EXP_NODE_TYPE_IDENTIFIER:
    //                 printf(" %s ", exp->constant.string_constant);
    //             break;

    //             case PRIMARY_EXP_NODE_TYPE_INTEGER_CONSTANT:
    //                 printf(" %d ", exp->constant.int_constant);
    //             break;

    //             case PRIMARY_EXP_NODE_TYPE_STRING_LITERAL:
    //                 printf(" \"%s\" ", exp->constant.string_constant);
    //             break;

    //             case PRIMARY_EXP_NODE_TYPE_EXPRESSION:
    //                 printf(" ( ");
    //                 traverse_exp_tree(exp->left);
    //                 printf(" ) ");
    //             break;
    //         }
    //     break;

    //     case EXP_NODE_TYPE_POSTFIX:
    //         // post_exp = (struct postfix_exp_node_t *)exp;
    //         traverse_exp_tree(exp->left);
    //         switch(exp->sub_type)
    //         {
    //             case POSTFIX_EXP_NODE_TYPE_FUNC_CALL:
    //                 printf(" ( ");
    //                 traverse_exp_tree(exp->right);
    //                 printf(" ) ");
    //             break;

    //             case POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX:
    //                 printf(" [ ");
    //                 traverse_exp_tree(exp->right);
    //                 printf(" ] ");
    //             break;

    //             case POSTFIX_EXP_NODE_TYPE_INCREMENT:
    //                 printf(" ++ ");
    //             break;

    //             case POSTFIX_EXP_NODE_TYPE_DECREMENT:
    //                 printf(" -- ");
    //             break;

    //             case POSTFIX_EXP_NODE_TYPE_ARROW:
    //                 printf(" -> ");
    //             break;

    //             case POSTFIX_EXP_NODE_TYPE_DOT:
    //                 printf(" . ");
    //             break;
    //         }
    //     break;

    //     case EXP_NODE_TYPE_UNARY:
    //         un_exp = (struct unary_exp_node_t *)exp;
    //         switch(un_exp->type)
    //         {
    //             case UNARY_EXP_NODE_TYPE_INCREMENT:
    //                 printf(" ++ ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_DECREMENT:
    //                 printf(" -- ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_ADDRESS_OF:
    //                 printf(" & ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_DEREFERENCE:
    //                 printf(" * ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_PLUS:
    //                 printf(" + ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_MINUS:
    //                 printf(" - ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_BITWISE_NOT:
    //                 printf(" ~ ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_LOGICAL_NOT:
    //                 printf(" ! ");
    //             break;

    //             case UNARY_EXP_NODE_TYPE_SIZEOF:
    //                 printf(" sizeof ");
    //             break;
    //         }
    //         traverse_exp_tree(un_exp->base.left);
    //     break;

    //     case EXP_NODE_TYPE_MULTIPLICATIVE:
    //         mul_exp = (struct multiplicative_exp_node_t *)exp;
    //         traverse_exp_tree(mul_exp->base.left);
    //         switch(mul_exp->type)
    //         {
    //             case MULTIPLICATIVE_EXP_NODE_TYPE_MULT:
    //                 printf(" * ");
    //             break;

    //             case MULTIPLICATIVE_EXP_NODE_TYPE_DIV:
    //                 printf(" / ");
    //             break;

    //             case MULTIPLICATIVE_EXP_NODE_TYPE_MOD:
    //                 printf(" %% ");
    //             break;
    //         }
    //         traverse_exp_tree(mul_exp->base.right);
    //     break;

    //     case EXP_NODE_TYPE_ADDITIVE:
    //         add_exp = (struct additive_exp_node_t *)exp;
    //         traverse_exp_tree(add_exp->base.left);
    //         switch(add_exp->type)
    //         {
    //             case ADDITIVE_EXP_NODE_TYPE_ADD:
    //                 printf(" + ");
    //             break;

    //             case ADDITIVE_EXP_NODE_TYPE_SUB:
    //                 printf(" * ");
    //             break;
    //         }
    //         traverse_exp_tree(add_exp->base.right);
    //     break;



    // }
}






