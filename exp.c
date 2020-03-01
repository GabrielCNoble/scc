#include "exp.h"


struct base_exp_node_t *exp(struct parser_t *parser)
{
    struct base_exp_node_t *exp;
    struct base_exp_node_t **exp_ptr = &exp;
    comma_exp(parser, &exp_ptr);
    return exp;
}

struct base_exp_node_t *new_node(uint32_t type)
{
    uint32_t size = sizeof(struct base_exp_node_t);
    struct base_exp_node_t *node;

    switch(type)
    {
        case EXP_NODE_TYPE_PRIMARY:
            size = sizeof(struct primary_exp_node_t);
        break;

        case EXP_NODE_TYPE_POSTFIX:
            size = sizeof(struct postfix_exp_node_t);
        break;

        case EXP_NODE_TYPE_UNARY:
            size = sizeof(struct unary_exp_node_t);
        break;

        case EXP_NODE_TYPE_MULTIPLICATIVE:
            size = sizeof(struct multiplicative_exp_node_t);
        break;

        case EXP_NODE_TYPE_ADDITIVE:
            size = sizeof(struct additive_exp_node_t);
        break;
    }

    node = calloc(1, size);
    node->type = type;
}

/* return... */
void comma_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    assignment_exp(parser, cur);
}


/* ','... */
void assignment_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    conditional_exp(parser, cur);
}

/* assignment '=', '+=', '-=', '*=', '/=', '%=', '<<=', '>>=', '&=', '|=', '^='... */
void conditional_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    logical_or_exp(parser, cur);
}

/* ternary '?:' */
void logical_or_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    logical_and_exp(parser, cur);
}

/* logical '||'... */
void logical_and_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    or_exp(parser, cur);
}

/* logical '&&'... */
void or_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    xor_exp(parser, cur);
}

/* bitwise '|'... */
void xor_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    and_exp(parser, cur);
}

/* bitwise '^'... */
void and_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    equality_exp(parser, cur);
}

/* bitwise '&' ... */
void equality_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    relational_exp(parser, cur);
}

/* relational '==', '!=' ... */
void relational_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    shift_exp(parser, cur);
}

/* relational '<', '<=', '>', '>=' ... */
void shift_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    additive_exp(parser, cur);
}

/* bitwise '<<', '>>' ... */
void additive_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    struct additive_exp_node_t *node;
    struct token_t *token;
    uint32_t node_type;

    multiplicative_exp(parser, cur);
    token = parser->current_token;

    if(token->token_type == TOKEN_PUNCTUATOR)
    {
        switch(token->token_name)
        {
            case TOKEN_PUNCTUATOR_PLUS:
                node_type = ADDITIVE_EXP_NODE_TYPE_ADD;
            break;

            case TOKEN_PUNCTUATOR_MINUS:
                node_type = ADDITIVE_EXP_NODE_TYPE_SUB;
            break;

            default:
                return;
        }

        advance_token(parser);
        node = (struct additive_exp_node_t *)new_node(EXP_NODE_TYPE_ADDITIVE);
        node->type = node_type;
        **cur = node;
        *cur = &node->base.next;
        additive_exp(parser, cur);
    }
}

/* arithmetic '+'. '-'... */
void multiplicative_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    struct multiplicative_exp_node_t *node;
    struct token_t *token;
    uint32_t node_type;

    cast_exp(parser, cur);
    token = parser->current_token;

    if(token->token_type == TOKEN_PUNCTUATOR)
    {
        switch(token->token_name)
        {
            case TOKEN_PUNCTUATOR_ASTERISC:
                node_type = MULTIPLICATIVE_EXP_NODE_TYPE_MULT;
            break;

            case TOKEN_PUNCTUATOR_SLASH:
                node_type = MULTIPLICATIVE_EXP_NODE_TYPE_DIV;
            break;

            case TOKEN_PUNCTUATOR_PERCENT:
                node_type = MULTIPLICATIVE_EXP_NODE_TYPE_MOD;
            break;

            default:
                return;
        }

        advance_token(parser);
        node = (struct multiplicative_exp_node_t *)new_node(EXP_NODE_TYPE_MULTIPLICATIVE);
        node->type = node_type;
        **cur = node;
        *cur = &node->base.next;
        multiplicative_exp(parser, cur);
    }
}

/* arithmetic '*', '/', '%' ... */
void cast_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    struct token_t *token;

    unary_exp(parser, cur);
//    token = parser->current_token;

    if(token->token_type == TOKEN_PUNCTUATOR)
    {
//        if(token->token_name == TOKEN_PUNCTUATOR_ASTERISC ||
//           token->token_name == TOKEN_PUNCTUATOR_SLASH)
//        {
//            advance_token(parser);
//            rec_result = exp_3(parser);
//
//            switch(token->token_name)
//            {
//                case TOKEN_PUNCTUATOR_ASTERISC:
//                    printf("reg%d = reg%d * reg%d\n", parser->reg_index, result.immediate, rec_result.immediate);
//                break;
//
//                case TOKEN_PUNCTUATOR_SLASH:
//                   printf("reg%d = reg%d / reg%d\n", parser->reg_index, result.immediate, rec_result.immediate);
//                break;
//            }
//
//            result.immediate = parser->reg_index;
//            parser->reg_index++;
//        }
    }
}

/* unary '*', '&', '+', '-', prefix '--', '++'... */
void unary_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    struct token_t *token;
    struct unary_exp_node_t *node;
    uint32_t node_type;
    uint32_t has_unary = 0;

    token = parser->current_token;

    switch(token->token_type)
    {
        case TOKEN_KEYWORD:
            has_unary = 1;
            switch(token->token_name)
            {
                case TOKEN_KEYWORD_SIZEOF:
                    node_type = UNARY_EXP_NODE_TYPE_SIZEOF;
                break;
            }
        break;

        case TOKEN_PUNCTUATOR:
            has_unary = 1;
            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_ASTERISC:
                    node_type = UNARY_EXP_NODE_TYPE_DEREFERENCE;
                break;

                case TOKEN_PUNCTUATOR_PLUS:
                    node_type = UNARY_EXP_NODE_TYPE_PLUS;
                break;

                case TOKEN_PUNCTUATOR_MINUS:
                    node_type = UNARY_EXP_NODE_TYPE_MINUS;
                break;

                case TOKEN_PUNCTUATOR_AMPERSAND:
                    node_type = UNARY_EXP_NODE_TYPE_ADDRESS_OF;
                break;

                case TOKEN_PUNCTUATOR_INCREMENT:
                    node_type = UNARY_EXP_NODE_TYPE_INCREMENT;
                break;

                case TOKEN_PUNCTUATOR_DECREMENT:
                    node_type = UNARY_EXP_NODE_TYPE_DECREMENT;
                break;

                case TOKEN_PUNCTUATOR_EXCLAMATION:
                    node_type = UNARY_EXP_NODE_TYPE_LOGICAL_NOT;
                break;

                case TOKEN_PUNCTUATOR_TILDE:
                    node_type = UNARY_EXP_NODE_TYPE_BITWISE_NOT;
                break;

                default:
                    has_unary = 0;
                break;
            }
        break;
    }

    if(has_unary)
    {
        node = (struct unary_exp_node_t *)new_node(EXP_NODE_TYPE_UNARY);
        node->type = node_type;
        **cur = node;
        *cur = &node->base.next;
        advance_token(parser);
        unary_exp(parser, cur);
    }

    postfix_exp(parser, cur);
}

/* '()', '[]', '{}', postfix '++', '--', '.', '->', ... */
void postfix_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    struct token_t *token;
    struct base_exp_node_t *rec_node;
    struct postfix_exp_node_t *node;
    uint32_t node_type;
    primary_exp(parser, cur);
    token = parser->current_token;

    if(token->token_type == TOKEN_PUNCTUATOR)
    {
        switch(token->token_name)
        {
            case TOKEN_PUNCTUATOR_OPARENTHESIS:
                node_type = POSTFIX_EXP_NODE_TYPE_FUNC_CALL;
            break;

            case TOKEN_PUNCTUATOR_OBRACKET:
                node_type = POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX;
            break;

            case TOKEN_PUNCTUATOR_INCREMENT:
                node_type = POSTFIX_EXP_NODE_TYPE_INCREMENT;
            break;

            case TOKEN_PUNCTUATOR_DECREMENT:
                node_type = POSTFIX_EXP_NODE_TYPE_DECREMENT;
            break;

            case TOKEN_PUNCTUATOR_DOT:
                node_type = POSTFIX_EXP_NODE_TYPE_DOT;
            break;

            case TOKEN_PUNCTUATOR_ARROW:
                node_type = POSTFIX_EXP_NODE_TYPE_ARROW;
            break;

            default:
                return;
            break;
        }

        advance_token(parser);
        node = (struct postfix_exp_node_t *)new_node(EXP_NODE_TYPE_POSTFIX);
        node->type = node_type;

        **cur = node;
        *cur = &node->base.next;

        if(node_type == POSTFIX_EXP_NODE_TYPE_FUNC_CALL ||
           node_type == POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX)
        {
            comma_exp(parser, cur);
            advance_token(parser);
        }
        else
        {
            postfix_exp(parser, cur);
        }
    }
}

/* literals, constants, identifiers ... */
void primary_exp(struct parser_t *parser, struct base_exp_node_t ***cur)
{
    struct token_t *token;
    struct primary_exp_node_t *node = NULL;
    token = parser->current_token;

    if(token)
    {
        switch(token->token_type)
        {
            case TOKEN_IDENTIFIER:
                node = (struct primary_exp_node_t *)new_node(EXP_NODE_TYPE_PRIMARY);
                node->type = PRIMARY_EXP_NODE_TYPE_IDENTIFIER;
                node->constant.string_constant = strdup(token->constant.string_constant);
                advance_token(parser);
            break;

            case TOKEN_CONSTANT:
                node = (struct primary_exp_node_t *)new_node(EXP_NODE_TYPE_PRIMARY);
                switch(token->token_name)
                {
                    case TOKEN_CONSTANT_INTEGER:
                        node->type = PRIMARY_EXP_NODE_TYPE_INTEGER_CONSTANT;
                        node->constant.int_constant = token->constant.int_constant;
                    break;
                }
                advance_token(parser);
            break;

            case TOKEN_STRING_LITERAL:
                node = (struct primary_exp_node_t *)new_node(EXP_NODE_TYPE_PRIMARY);
                node->type = PRIMARY_EXP_NODE_TYPE_STRING_LITERAL;
                node->constant.string_constant = strdup(token->constant.string_constant);
                advance_token(parser);
            break;

            case TOKEN_PUNCTUATOR:
                if(token->token_name == TOKEN_PUNCTUATOR_OPARENTHESIS)
                {
                    advance_token(parser);
                    comma_exp(parser, cur);
                    advance_token(parser);
                    return;
                }
            break;

            default:
                return;
        }

        **cur = node;
        *cur = &node->base.next;
    }
}
