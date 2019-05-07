#include "exp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static struct token_t *next_token = NULL;

//
//struct token_t *get_token()
//{
//    return next_token;
//}
//
//void advance_token()
//{
//    if(next_token)
//    {
//        next_token = next_token->next;
//    }
//}

//void parse_statement(struct token_t *tokens)
//void expression(struct token_t *tokens)
//{
//    struct token_t *token;
//    struct exp_result_t result;
//
//    token = tokens;
//    expression7(&result, &token);
//
//    if(result.result_type == EXP_RESULT_TYPE_VARIABLE)
//    {
//        printf("-->%d\n", result.result_value.var->value.ivalue);
//    }
//    else if(result.result_type == EXP_RESULT_TYPE_LITERAL)
//    {
//        printf("-->%d\n", result.result_value.int_result);
//    }
//    else
//    {
//        printf("-->%s\n", result.text);
//    }
//
//
//    //free_tokens(tokens);
//}

/* comma... */
//void expression7(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//
//    struct exp_result_t temp;
//    //char error_buffer[128];
//    int temp_value;
//
//    expression6(result, tokens);
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token == TOKEN_COMMA))
//    {
//        //advance_token();
//        *tokens = token->next;
//        expression7(&temp, tokens);
//
//        /* the comma operator pretty much just throws the temp value away,
//        given that the expression is processed left-to-right, but the comma
//        is right-to-left associative, which means the first thing read before
//        the first comma is the final value of the expression... */
//
//        token = get_token();
//    }
//}


/* assignments... */
//void expression6(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
////    struct var_t *var;
////    struct var_t *temp_var;
//
//    struct exp_result_t temp;
//    int temp_value;
//
//    char error_buffer[128];
//
//    expression4(result, tokens);
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_ASSIGN || token->token == TOKEN_PLUS_ASSIGN ||
//                                                                                        token->token == TOKEN_MINUS_ASSIGN ||
//                                                                                        token->token == TOKEN_MUL_ASSIGN ||
//                                                                                        token->token == TOKEN_DIV_ASSIGN)))
//    {
//        //advance_token();
//        *tokens = token->next;
//        expression6(&temp, tokens);
//
//        /* left side exp... */
//        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!result->result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(error_buffer, "undefined identifier [%s]", result->text);
//                strcpy(result->text, error_buffer);
//                return;
//            }
//        }
//        else if(result->result_type == EXP_RESULT_TYPE_LITERAL)
//        {
//            result->result_type = EXP_RESULT_TYPE_ERROR;
//            sprintf(result->text, "literal cannot be lvalue");
//            return;
//        }
//
//
//        /* right exp... */
//        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!temp.result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(result->text, "undefined identifier [%s]", temp.text);
//                return;
//            }
//            else
//            {
//                temp_value = temp.result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            temp_value = temp.result_value.int_result;
//        }
//
//        switch(token->token)
//        {
//            case TOKEN_ASSIGN:
//                result->result_value.var->value.ivalue = temp_value;
//            break;
//
//            case TOKEN_PLUS_ASSIGN:
//                result->result_value.var->value.ivalue += temp_value;
//            break;
//
//            case TOKEN_MINUS_ASSIGN:
//                result->result_value.var->value.ivalue -= temp_value;
//            break;
//
//            case TOKEN_MUL_ASSIGN:
//                result->result_value.var->value.ivalue *= temp_value;
//            break;
//
//            case TOKEN_DIV_ASSIGN:
//                result->result_value.var->value.ivalue /= temp_value;
//            break;
//        }
//
//        result->result_type = EXP_RESULT_TYPE_LITERAL;
//        result->result_value.int_result = result->result_value.var->value.ivalue;
//
//        //token = get_token();
//        token = *tokens;
//    }
//}

///* var decls... */
//void expression5(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//    struct var_t *var;
//    struct exp_result_t temp_result;
//
//    int var_type;
//
//    expression4(result);
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token != TOKEN_SEMICOLON && token->token == TOKEN_RESERVED))
//    {
//        //advance_token();
//        *tokens = token->next;
//
//        expression4(&temp_result);
//
//        switch(temp_result.result_type)
//        {
//            case EXP_RESULT_TYPE_VARIABLE:
//                if(!temp_result.result_value.var)
//                {
//                    /* var declaration... */
//
//                    switch(token->reserved_token)
//                    {
//                        case RESERVED_TOKEN_INT:
//                            var_type = VAR_TYPE_INT;
//                        break;
//                    }
//
//                    create_var(temp_result.text, var_type);
//
//                    var = get_var(temp_result.text);
//
//                    result->result_type = EXP_RESULT_TYPE_VARIABLE;
//                    result->result_value.var = var;
//                    strcpy(result->text, temp_result.text);
//                }
//                else
//                {
//                    result->result_type = EXP_RESULT_TYPE_ERROR;
//                    sprintf(result->text, "redeclaration of identifier [%s]", temp_result.text);
//                }
//            break;
//
//            case EXP_RESULT_TYPE_LITERAL:
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(result->text, "identifier starting with number");
//            break;
//        }
//
//        return;
//    }
//}

/* == */
//void expression4(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//
//    struct exp_result_t temp;
//    char error_buffer[128];
//
//    int result_value;
//    int temp_value;
//
//    expression3(result, tokens);
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_EQUALS || token->token == TOKEN_NOT_EQUALS)))
//    {
//        //advance_token();
//        *tokens = token->next;
//        expression3(&temp, tokens);
//
//        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!result->result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(error_buffer, "undefined identifier [%s]", result->text);
//                strcpy(result->text, error_buffer);
//            }
//            else
//            {
//                result_value = result->result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            result_value = result->result_value.int_result;
//        }
//
//
//
//        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!temp.result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(result->text, "undefined identifier [%s]", temp.text);
//            }
//            else
//            {
//                temp_value = temp.result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            temp_value = temp.result_value.int_result;
//        }
//
//
//
//        switch(token->token)
//        {
//            case TOKEN_EQUALS:
//                result_value = result_value == temp_value;
//            break;
//
//            case TOKEN_NOT_EQUALS:
//                result_value = result_value != temp_value;
//            break;
//        }
//
//        result->result_type = EXP_RESULT_TYPE_LITERAL;
//        result->result_value.int_result = result_value;
//
//        //token = get_token();
//        token = *tokens;
//    }
//}

/* +- */
//void expression3(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//    struct exp_result_t temp;
//
//    char error_buffer[128];
//
//    expression2(result, tokens);
//
//    int result_value;
//    int temp_value;
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_PLUS || token->token == TOKEN_MINUS)))
//    {
//        //advance_token();
//        *tokens = token->next;
//
//        expression2(&temp, tokens);
//
//
//
//        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!result->result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(error_buffer, "undefined identifier [%s]", result->text);
//                strcpy(result->text, error_buffer);
//            }
//            else
//            {
//                result_value = result->result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            result_value = result->result_value.int_result;
//        }
//
//
//
//        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!temp.result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(result->text, "undefined identifier [%s]", temp.text);
//            }
//            else
//            {
//                temp_value = temp.result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            temp_value = temp.result_value.int_result;
//        }
//
//        switch(token->token)
//        {
//            case TOKEN_PLUS:
//                result_value += temp_value;
//            break;
//
//            case TOKEN_MINUS:
//                result_value -= temp_value;
//            break;
//        }
//
//        result->result_type = EXP_RESULT_TYPE_LITERAL;
//        result->result_value.int_result = result_value;
//        token = *tokens;
//        //token = get_token();
//    }
//}

/* / * */
//void expression2(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//    struct exp_result_t temp;
//
//    char error_buffer[512];
//
//    int result_value;
//    int temp_value;
//
//    expression1(result, tokens);
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_ASTERISC || token->token == TOKEN_DIV)))
//    {
//        //advance_token();
//        *tokens = token->next;
//        expression1(&temp, tokens);
//
//
//
//        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!result->result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(error_buffer, "undefined identifier [%s]", result->text);
//                strcpy(result->text, error_buffer);
//            }
//            else
//            {
//                result_value = result->result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            result_value = result->result_value.int_result;
//        }
//
//
//
//        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!temp.result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(result->text, "undefined identifier [%s]", temp.text);
//            }
//            else
//            {
//                temp_value = temp.result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            temp_value = temp.result_value.int_result;
//        }
//
//        switch(token->token)
//        {
//            case TOKEN_ASTERISC:
//                result_value *= temp_value;
//            break;
//
//            case TOKEN_DIV:
//                result_value /= temp_value;
//            break;
//        }
//
//        result->result_type = EXP_RESULT_TYPE_LITERAL;
//        result->result_value.int_result = result_value;
//        token = *tokens;
//        //token = get_token();
//    }
//}

/* unary +- */
//void expression1(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//    int result_value;
//
//    char error_buffer[512];
//
//    int token_type = TOKEN_UNKNOWN;
//
//    //token = get_token();
//    token = *tokens;
//
//    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_PLUS || token->token == TOKEN_MINUS)))
//    {
//        token_type = token_type == TOKEN_MINUS ? TOKEN_UNKNOWN : TOKEN_MINUS;
//
//        *tokens = token->next;
//        token = token->next;
//
//        /*advance_token();
//        token = get_token();*/
//    }
//
//    expression0(result, tokens);
//
//    if(token_type == TOKEN_MINUS)
//    {
//        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
//        {
//            if(!result->result_value.var)
//            {
//                result->result_type = EXP_RESULT_TYPE_ERROR;
//                sprintf(error_buffer, "undefined identifier [%s]", result->text);
//                strcpy(result->text, error_buffer);
//                return;
//            }
//            else
//            {
//                result_value = result->result_value.var->value.ivalue;
//            }
//        }
//        else
//        {
//            result_value = result->result_value.int_result;
//        }
//
//        result_value = -result_value;
//        result->result_type = EXP_RESULT_TYPE_LITERAL;
//        result->result_value.int_result = result_value;
//    }
//}

/* () number... */
//void expression0(struct exp_result_t *result, struct token_t **tokens)
//{
//    struct token_t *token;
//    struct var_t *var;
//
//    struct exp_result_t temp;
//    int var_type;
//
//    //token = get_token();
//    token = *tokens;
//
//    if(token && token->token != TOKEN_SEMICOLON)
//    {
//        if(token->token == TOKEN_OPARENTHESIS)
//        {
//            //advance_token();
//            *tokens = token->next;
//            expression7(result, tokens);
//            token = *tokens;
//            *tokens = token->next;
//            //advance_token();
//        }
//        else
//        {
//            if(token->token == TOKEN_NUMBER)
//            {
//                result->result_type = EXP_RESULT_TYPE_LITERAL;
//                result->result_value.int_result = atoi(token->text);
//                *tokens = token->next;
//                //advance_token();
//            }
//            else if(token->token == TOKEN_ID)
//            {
//                var = get_var(token->text);
//
//                result->result_type = EXP_RESULT_TYPE_VARIABLE;
//                result->result_value.var = var;
//                strcpy(result->text, token->text);
//                *tokens = token->next;
//                //advance_token();
//            }
//        }
//    }
//}

/* return... */
struct exp_result_t exp_16(struct parser_t *parser)
{
    return exp_15(parser);
}


/* ','... */
struct exp_result_t exp_15(struct parser_t *parser)
{
    return exp_14(parser);
}

/* assignment '=', '+=', '-=', '*=', '/=', '%=', '<<=', '>>=', '&=', '|=', '^='... */
struct exp_result_t exp_14(struct parser_t *parser)
{
    return exp_13(parser);
}

/* ternary '?:' */
struct exp_result_t exp_13(struct parser_t *parser)
{
    return exp_12(parser);
}

/* logical '||'... */
struct exp_result_t exp_12(struct parser_t *parser)
{
    return exp_11(parser);
}

/* logical '&&'... */
struct exp_result_t exp_11(struct parser_t *parser)
{
    return exp_10(parser);
}

/* bitwise '|'... */
struct exp_result_t exp_10(struct parser_t *parser)
{
    return exp_9(parser);
}

/* bitwise '^'... */
struct exp_result_t exp_9(struct parser_t *parser)
{
    return exp_8(parser);
}

/* bitwise '&' ... */
struct exp_result_t exp_8(struct parser_t *parser)
{
    return exp_7(parser);
}

/* relational '==', '!=' ... */
struct exp_result_t exp_7(struct parser_t *parser)
{
    return exp_6(parser);
}

/* relational '<', '<=', '>', '>=' ... */
struct exp_result_t exp_6(struct parser_t *parser)
{
    return exp_5(parser);
}

/* bitwise '<<', '>>' ... */
struct exp_result_t exp_5(struct parser_t *parser)
{
    return exp_4(parser);
}

/* arithmetic '+'. '-'... */
struct exp_result_t exp_4(struct parser_t *parser)
{
    struct token_t *token;
    struct exp_result_t result;
    struct exp_result_t rec_result;

    result = exp_3(parser);
    token = parser->current_token;


    if(token->token_type == TOKEN_PUNCTUATOR)
    {
        if(token->token_name == TOKEN_PUNCTUATOR_PLUS || token->token_name == TOKEN_PUNCTUATOR_MINUS)
        {
            advance_token(parser);

            rec_result = exp_3(parser);

            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_PLUS:
                    printf("reg%d = reg%d + reg%d\n", parser->reg_index, result.immediate, rec_result.immediate);
                break;

                case TOKEN_PUNCTUATOR_MINUS:
                    printf("reg%d = reg%d - reg%d\n", parser->reg_index, result.immediate, rec_result.immediate);
                break;
            }

            result.immediate = parser->reg_index;
            parser->reg_index++;
        }
    }

    return result;
}

/* arithmetic '*', '/', '%' ... */
struct exp_result_t exp_3(struct parser_t *parser)
{
    struct token_t *token;
    struct exp_result_t result;
    struct exp_result_t rec_result;

    result = exp_2(parser);

    token = parser->current_token;

    if(token->token_type == TOKEN_PUNCTUATOR)
    {
        if(token->token_name == TOKEN_PUNCTUATOR_ASTERISC ||
           token->token_name == TOKEN_PUNCTUATOR_SLASH)
        {
            advance_token(parser);
            rec_result = exp_3(parser);

            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_ASTERISC:
                    printf("reg%d = reg%d * reg%d\n", parser->reg_index, result.immediate, rec_result.immediate);
                break;

                case TOKEN_PUNCTUATOR_SLASH:
                   printf("reg%d = reg%d / reg%d\n", parser->reg_index, result.immediate, rec_result.immediate);
                break;
            }

            result.immediate = parser->reg_index;
            parser->reg_index++;
        }
    }


    return result;
}

/* unary '*', '&', '+', '-', prefix '--', '++'... */
struct exp_result_t exp_2(struct parser_t *parser)
{
    struct token_t *token;
    struct exp_result_t result;

    token = parser->current_token;

    switch(token->token_type)
    {
//            case TOKEN_KEYWORD:
//                switch(token->token_name)
//                {
//                    case TOKEN_KEYWORD_SIZEOF:
//                        node = calloc(sizeof(struct ast_node_t), 1);
//                        node->token = token;
//
//                        advance_token(parser);
//                        node->left = exp_2(parser);
//                    break;
//
//                    default:
//                        node = exp_2(parser);
//                    break;
//                }
//            break;

        case TOKEN_PUNCTUATOR:

            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_ASTERISC:
                case TOKEN_PUNCTUATOR_PLUS:
                case TOKEN_PUNCTUATOR_MINUS:
                case TOKEN_PUNCTUATOR_AMPERSAND:
                case TOKEN_PUNCTUATOR_INCREMENT:
                case TOKEN_PUNCTUATOR_DECREMENT:

                    result = exp_2(parser);

                    switch(token->token_name)
                    {
                        case TOKEN_PUNCTUATOR_ASTERISC:
                            printf("reg%d = [reg%d]\n", result.immediate, result.immediate);
                        break;

                        case TOKEN_PUNCTUATOR_PLUS:
                            printf("reg%d = +reg%d\n", result.immediate, result.immediate);
                        break;

                        case TOKEN_PUNCTUATOR_MINUS:
                            printf("reg%d = -reg%d\n", result.immediate, result.immediate);
                        break;

                        case TOKEN_PUNCTUATOR_AMPERSAND:
                            printf("reg%d = &reg%d\n", result.immediate, result.immediate);
                        break;

                        case TOKEN_PUNCTUATOR_INCREMENT:

                        break;

                        case TOKEN_PUNCTUATOR_DECREMENT:

                        break;
                    }

                break;

                default:
                    result = exp_1(parser);
                break;
            }
        break;

        default:
            result = exp_1(parser);
        break;
    }


    return result;
}

/* '()', '[]', '{}', postfix '++', '--', '.', '->', ... */
struct exp_result_t exp_1(struct parser_t *parser)
{
    struct token_t *token;
    struct exp_result_t result;

    result = exp_0(parser);
    token = parser->current_token;

    if(token->token_type == TOKEN_PUNCTUATOR)
    {
        switch(token->token_name)
        {
            case TOKEN_PUNCTUATOR_OPARENTHESIS:
            case TOKEN_PUNCTUATOR_OBRACKET:
            case TOKEN_PUNCTUATOR_OBRACE:
                advance_token(parser);
                result = exp_16(parser);
                advance_token(parser);
            break;

            case TOKEN_PUNCTUATOR_INCREMENT:
                //printf("reg%d++\n", result.immediate);
                advance_token(parser);
            break;

            case TOKEN_PUNCTUATOR_DECREMENT:
                //printf("reg%d--\n", result.immediate);
                advance_token(parser);
            break;

            case TOKEN_PUNCTUATOR_DOT:

            break;

            case TOKEN_PUNCTUATOR_ARROW:

            break;
        }
    }


    return result;
}

/* literals, identifiers ... */
struct exp_result_t exp_0(struct parser_t *parser)
{
    struct token_t *token;
    struct exp_result_t result;

    token = parser->current_token;

    if(token)
    {
        switch(token->token_type)
        {
            case TOKEN_IDENTIFIER:
                //printf("reg%d = mem\n", parser->reg_index);
                //result.immediate = parser->reg_index;
                //parser->reg_index++;
                advance_token(parser);
            break;

            case TOKEN_CONSTANT:
                //printf("reg%d = constant\n", parser->reg_index);
                //result.immediate = parser->reg_index;
                //parser->reg_index++;
                advance_token(parser);
            break;

            default:
                /* if it falls here, something is wrong... */
            break;
        }
    }

    return result;
}


//
//void traverse_ast(struct ast_node_t *ast)
//{
//    static int depth = -1;
//    int i;
//
//    depth++;
//
//    if(ast)
//    {
//        for(i = 0; i < depth; i++)
//        {
//            putchar(' ');
//        }
//        translate_token(ast->token);
//        traverse_ast(ast->left);
//        traverse_ast(ast->right);
//    }
//    depth--;
//}












