#include "exp.h"
#include "parse.h"

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

    return node;
}


struct base_exp_node_t *parse_exp(struct parser_t *parser)
{
    return comma_exp(parser);
}

struct base_exp_node_t *comma_exp(struct parser_t *parser)
{
    return assignment_exp(parser);
}

struct base_exp_node_t *assignment_exp(struct parser_t *parser)
{
    return conditional_exp(parser);
}

struct base_exp_node_t *conditional_exp(struct parser_t *parser)
{
    return logical_or_exp(parser);
}

struct base_exp_node_t *logical_or_exp(struct parser_t *parser)
{
    return logical_and_exp(parser);
}

struct base_exp_node_t *logical_and_exp(struct parser_t *parser)
{
    return or_exp(parser);
}

struct base_exp_node_t *or_exp(struct parser_t *parser)
{
    return xor_exp(parser);
}

struct base_exp_node_t *xor_exp(struct parser_t *parser)
{
    return and_exp(parser);
}

struct base_exp_node_t *and_exp(struct parser_t *parser)
{
    return equality_exp(parser);
}

struct base_exp_node_t *equality_exp(struct parser_t *parser)
{
    return relational_exp(parser);
}

struct base_exp_node_t *relational_exp(struct parser_t *parser)
{
    return shift_exp(parser);
}

struct base_exp_node_t *shift_exp(struct parser_t *parser)
{
    return additive_exp(parser);
}

struct base_exp_node_t *additive_exp(struct parser_t *parser)
{
    struct additive_exp_node_t *node;
    struct token_t *token;
    uint32_t node_type;

    return multiplicative_exp(parser);

//    multiplicative_exp(parser, cur);
//    token = parser->current_token;
//
//    if(token->type == TOKEN_PUNCTUATOR)
//    {
//        switch(token->name)
//        {
//            case TOKEN_PUNCTUATOR_PLUS:
//                node_type = ADDITIVE_EXP_NODE_TYPE_ADD;
//            break;
//
//            case TOKEN_PUNCTUATOR_MINUS:
//                node_type = ADDITIVE_EXP_NODE_TYPE_SUB;
//            break;
//
//            default:
//                return;
//        }
//
//        advance_token(parser);
//        node = (struct additive_exp_node_t *)new_node(EXP_NODE_TYPE_ADDITIVE);
//        node->type = node_type;
//        **cur = node;
//        *cur = &node->base.next;
//        additive_exp(parser, cur);
//    }
}

struct base_exp_node_t *multiplicative_exp(struct parser_t *parser)
{
    struct multiplicative_exp_node_t *mul_exp;
    struct base_exp_node_t *left_exp;
    struct token_t *token;
    uint32_t node_type;
    uint32_t has_mult = 0;

    left_exp = cast_exp(parser);
    token = parser->cur_token;

    if(token->type == TOKEN_PUNCTUATOR)
    {
        has_mult = 1;
        switch(token->name)
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
                has_mult = 0;
            break;
        }
    }

    if(has_mult)
    {
        advance_token(parser);
        mul_exp = (struct multiplicative_exp_node_t *)new_node(EXP_NODE_TYPE_MULTIPLICATIVE);
        mul_exp->type = node_type;
        mul_exp->base.left = left_exp;
        mul_exp->base.right = multiplicative_exp(parser);
        left_exp = (struct base_exp_node_t *)mul_exp;
    }

    return left_exp;
}

struct base_exp_node_t *cast_exp(struct parser_t *parser)
{
    struct token_t *token;

    return unary_exp(parser);
//    token = parser->current_token;

    if(token->type == TOKEN_PUNCTUATOR)
    {
//        if(token->name == TOKEN_PUNCTUATOR_ASTERISC ||
//           token->name == TOKEN_PUNCTUATOR_SLASH)
//        {
//            advance_token(parser);
//            rec_result = exp_3(parser);
//
//            switch(token->name)
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

struct base_exp_node_t *unary_exp(struct parser_t *parser)
{
    struct token_t *token;
    struct unary_exp_node_t *un_exp = NULL;
    uint32_t node_type;
    uint32_t has_unary = 0;

    token = parser->cur_token;

    switch(token->type)
    {
        case TOKEN_KEYWORD:
            has_unary = 1;
            switch(token->name)
            {
                case TOKEN_KEYWORD_SIZEOF:
                    node_type = UNARY_EXP_NODE_TYPE_SIZEOF;
                break;
            }
        break;

        case TOKEN_PUNCTUATOR:
            has_unary = 1;
            switch(token->name)
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
        advance_token(parser);
        un_exp = (struct unary_exp_node_t *)new_node(EXP_NODE_TYPE_UNARY);
        un_exp->type = node_type;
        un_exp->base.left = unary_exp(parser);
    }
    else
    {
        un_exp = (struct unary_exp_node_t *)postfix_exp(parser);
    }

    return (struct base_exp_node_t *)un_exp;
}

struct base_exp_node_t *postfix_exp(struct parser_t *parser)
{
    struct token_t *token;
    struct postfix_exp_node_t *pos_exp;
    struct base_exp_node_t *left_exp;
    uint32_t node_type;
    uint32_t has_postfix = 0;

    left_exp = primary_exp(parser);
    token = parser->cur_token;

    if(token->type == TOKEN_PUNCTUATOR)
    {
        has_postfix = 1;
        switch(token->name)
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
                has_postfix = 0;
            break;
        }
    }

    if(has_postfix)
    {
        advance_token(parser);
        pos_exp = (struct postfix_exp_node_t *)new_node(EXP_NODE_TYPE_POSTFIX);
        pos_exp->type = node_type;
        pos_exp->base.left = left_exp;
        if(node_type == POSTFIX_EXP_NODE_TYPE_FUNC_CALL || node_type == POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX)
        {
            pos_exp->base.right = comma_exp(parser);
            advance_token(parser);
        }
        else
        {
            pos_exp->base.right = postfix_exp(parser);
        }

        left_exp = (struct base_exp_node_t *)pos_exp;
    }

    return left_exp;
}

struct base_exp_node_t *primary_exp(struct parser_t *parser)
{
    struct token_t *token;
    struct primary_exp_node_t *node = NULL;
    token = parser->cur_token;

    if(token)
    {
        switch(token->type)
        {
            case TOKEN_IDENTIFIER:
                node = (struct primary_exp_node_t *)new_node(EXP_NODE_TYPE_PRIMARY);
                node->type = PRIMARY_EXP_NODE_TYPE_IDENTIFIER;
                node->constant.string_constant = strdup(token->constant.string_constant);
                advance_token(parser);
            break;

            case TOKEN_CONSTANT:
                node = (struct primary_exp_node_t *)new_node(EXP_NODE_TYPE_PRIMARY);
                switch(token->name)
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
                if(token->name == TOKEN_PUNCTUATOR_OPARENTHESIS)
                {
                    node = (struct primary_exp_node_t *)new_node(EXP_NODE_TYPE_PRIMARY);
                    node->type = PRIMARY_EXP_NODE_TYPE_EXPRESSION;
                    advance_token(parser);
                    node->base.left = comma_exp(parser);
                    advance_token(parser);
                }
            break;
        }
    }

    return (struct base_exp_node_t *)node;
}

void traverse_exp_tree(struct base_exp_node_t *exp)
{
    struct primary_exp_node_t *prim_exp;
    struct postfix_exp_node_t *post_exp;
    struct unary_exp_node_t *un_exp;
    struct multiplicative_exp_node_t *mul_exp;
    struct additive_exp_node_t *add_exp;

    if(!exp)
    {
        return;
    }

    switch(exp->type)
    {
        case EXP_NODE_TYPE_PRIMARY:
            prim_exp = (struct primary_exp_node_t *)exp;
            switch(prim_exp->type)
            {
                case PRIMARY_EXP_NODE_TYPE_IDENTIFIER:
                    printf(" %s ", prim_exp->constant.string_constant);
                break;

                case PRIMARY_EXP_NODE_TYPE_INTEGER_CONSTANT:
                    printf(" %d ", prim_exp->constant.int_constant);
                break;

                case PRIMARY_EXP_NODE_TYPE_STRING_LITERAL:
                    printf(" \"%s\" ", prim_exp->constant.string_constant);
                break;

                case PRIMARY_EXP_NODE_TYPE_EXPRESSION:
                    printf(" ( ");
                    traverse_exp_tree(prim_exp->base.left);
                    printf(" ) ");
                break;
            }
        break;

        case EXP_NODE_TYPE_POSTFIX:
            post_exp = (struct postfix_exp_node_t *)exp;
            traverse_exp_tree(post_exp->base.left);
            switch(post_exp->type)
            {
                case POSTFIX_EXP_NODE_TYPE_FUNC_CALL:
                    printf(" ( ");
                    traverse_exp_tree(post_exp->base.right);
                    printf(" ) ");
                break;

                case POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX:
                    printf(" [ ");
                    traverse_exp_tree(post_exp->base.right);
                    printf(" ] ");
                break;

                case POSTFIX_EXP_NODE_TYPE_INCREMENT:
                    printf(" ++ ");
                break;

                case POSTFIX_EXP_NODE_TYPE_DECREMENT:
                    printf(" -- ");
                break;

                case POSTFIX_EXP_NODE_TYPE_ARROW:
                    printf(" -> ");
                break;

                case POSTFIX_EXP_NODE_TYPE_DOT:
                    printf(" . ");
                break;
            }
        break;

        case EXP_NODE_TYPE_UNARY:
            un_exp = (struct unary_exp_node_t *)exp;
            switch(un_exp->type)
            {
                case UNARY_EXP_NODE_TYPE_INCREMENT:
                    printf(" ++ ");
                break;

                case UNARY_EXP_NODE_TYPE_DECREMENT:
                    printf(" -- ");
                break;

                case UNARY_EXP_NODE_TYPE_ADDRESS_OF:
                    printf(" & ");
                break;

                case UNARY_EXP_NODE_TYPE_DEREFERENCE:
                    printf(" * ");
                break;

                case UNARY_EXP_NODE_TYPE_PLUS:
                    printf(" + ");
                break;

                case UNARY_EXP_NODE_TYPE_MINUS:
                    printf(" - ");
                break;

                case UNARY_EXP_NODE_TYPE_BITWISE_NOT:
                    printf(" ~ ");
                break;

                case UNARY_EXP_NODE_TYPE_LOGICAL_NOT:
                    printf(" ! ");
                break;

                case UNARY_EXP_NODE_TYPE_SIZEOF:
                    printf(" sizeof ");
                break;
            }
            traverse_exp_tree(un_exp->base.left);
        break;

        case EXP_NODE_TYPE_MULTIPLICATIVE:
            mul_exp = (struct multiplicative_exp_node_t *)exp;
            traverse_exp_tree(mul_exp->base.left);
            switch(mul_exp->type)
            {
                case MULTIPLICATIVE_EXP_NODE_TYPE_MULT:
                    printf(" * ");
                break;

                case MULTIPLICATIVE_EXP_NODE_TYPE_DIV:
                    printf(" / ");
                break;

                case MULTIPLICATIVE_EXP_NODE_TYPE_MOD:
                    printf(" %% ");
                break;
            }
            traverse_exp_tree(mul_exp->base.right);
        break;

        case EXP_NODE_TYPE_ADDITIVE:
            add_exp = (struct additive_exp_node_t *)exp;
            traverse_exp_tree(add_exp->base.left);
            switch(add_exp->type)
            {
                case ADDITIVE_EXP_NODE_TYPE_ADD:
                    printf(" + ");
                break;

                case ADDITIVE_EXP_NODE_TYPE_SUB:
                    printf(" * ");
                break;
            }
            traverse_exp_tree(add_exp->base.right);
        break;



    }
}






