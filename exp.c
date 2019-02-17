#include "exp.h"
#include "var.h"
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
struct ast_node_t *exp_16(struct token_t **tokens)
{
    struct token_t *token;
    struct ast_node_t *node;
    struct ast_node_t *child;

    token = *tokens;

    if(token)
    {
        if(token->token_type == TOKEN_KEYWORD)
        {
            switch(token->token_name)
            {
                case TOKEN_KEYWORD_RETURN:
                case TOKEN_KEYWORD_INT:
                case TOKEN_KEYWORD_FLOAT:
                case TOKEN_KEYWORD_CHAR:
                case TOKEN_KEYWORD_SHORT:
                case TOKEN_KEYWORD_VOID:
                    node = calloc(sizeof(struct ast_node_t), 1);
                    node->token = token;

                    *tokens = token->next;
                    node->left = exp_16(tokens);
                break;

                default:
                    node = exp_15(tokens);
                break;
            }
        }
        else
        {
            node = exp_15(tokens);
        }

    }
    else
    {
        node = exp_15(tokens);
    }

    return node;
}


/* ','... */
struct ast_node_t *exp_15(struct token_t **tokens)
{
    return exp_14(tokens);
}

/* assignment '=', '+=', '-=', '*=', '/=', '%=', '<<=', '>>=', '&=', '|=', '^='... */
struct ast_node_t *exp_14(struct token_t **tokens)
{
    return exp_13(tokens);
}

/* ternary '?:' */
struct ast_node_t *exp_13(struct token_t **tokens)
{
    return exp_12(tokens);
}

/* logical '||'... */
struct ast_node_t *exp_12(struct token_t **tokens)
{
    return exp_11(tokens);
}

/* logical '&&'... */
struct ast_node_t *exp_11(struct token_t **tokens)
{
    return exp_10(tokens);
}

/* bitwise '|'... */
struct ast_node_t *exp_10(struct token_t **tokens)
{
    return exp_9(tokens);
}

/* bitwise '^'... */
struct ast_node_t *exp_9(struct token_t **tokens)
{
    return exp_8(tokens);
}

/* bitwise '&' ... */
struct ast_node_t *exp_8(struct token_t **tokens)
{
    return exp_7(tokens);
}

/* relational '==', '!=' ... */
struct ast_node_t *exp_7(struct token_t **tokens)
{
    return exp_6(tokens);
}

/* relational '<', '<=', '>', '>=' ... */
struct ast_node_t *exp_6(struct token_t **tokens)
{
    return exp_5(tokens);
}

/* bitwise '<<', '>>' ... */
struct ast_node_t *exp_5(struct token_t **tokens)
{
    return exp_4(tokens);
}

/* arithmetic '+'. '-'... */
struct ast_node_t *exp_4(struct token_t **tokens)
{
    struct token_t *token;
    struct ast_node_t *node = NULL;
    struct ast_node_t *child = NULL;

    child = exp_3(tokens);
    token = *tokens;

    node = child;

    if(token)
    {
        if(token->token_type == TOKEN_PUNCTUATOR)
        {
            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_PLUS:
                case TOKEN_PUNCTUATOR_MINUS:
                    node = calloc(sizeof(struct ast_node_t), 1);

                    node->left = child;
                    node->token = token;

                    *tokens = token->next;

                    node->right = exp_4(tokens);
                break;
            }
        }
    }

    return node;
}

/* arithmetic '*', '/', '%' ... */
struct ast_node_t *exp_3(struct token_t **tokens)
{
    struct token_t *token;
    struct ast_node_t *node = NULL;
    struct ast_node_t *child = NULL;

    child = exp_2(tokens);
    token = *tokens;

    node = child;

    if(token)
    {
        if(token->token_type == TOKEN_PUNCTUATOR)
        {
            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_ASTERISC:
                case TOKEN_PUNCTUATOR_SLASH:
                    *tokens = token->next;
                    node = calloc(sizeof(struct ast_node_t), 1);
                    node->token = token;
                    node->left = child;

                    child = exp_3(tokens);
                    node->right = child;
                break;
            }
        }
    }

    return node;
}

/* unary '*', '&', '+', '-' ... */
struct ast_node_t *exp_2(struct token_t **tokens)
{
    struct token_t *token;
    struct ast_node_t *node = NULL;
    struct ast_node_t *child = NULL;

    token = *tokens;

    if(token)
    {
        switch(token->token_type)
        {
            case TOKEN_KEYWORD:
                switch(token->token_name)
                {
                    case TOKEN_KEYWORD_SIZEOF:
                        node = calloc(sizeof(struct ast_node_t), 1);
                        node->token = token;

                        *tokens = token->next;
                        node->left = exp_2(tokens);
                    break;

                    default:
                        node = exp_2(tokens);
                    break;
                }
            break;

            case TOKEN_PUNCTUATOR:
                switch(token->token_name)
                {
                    case TOKEN_PUNCTUATOR_ASTERISC:
                    case TOKEN_PUNCTUATOR_PLUS:
                    case TOKEN_PUNCTUATOR_MINUS:
                    case TOKEN_PUNCTUATOR_AMPERSAND:
                        node = calloc(sizeof(struct ast_node_t), 1);
                        node->token = token;

                        *tokens = token->next;
                        node->left = exp_2(tokens);
                    break;

                    default:
                        node = exp_1(tokens);
                    break;
                }
            break;

            default:
                node = exp_1(tokens);
            break;
        }
    }
    else
    {
        node = exp_1(tokens);
    }

    return node;
}

/* '()', '[]', '{}', postfix '++', '--', '.', '->', ... */
struct ast_node_t *exp_1(struct token_t **tokens)
{
    struct token_t *token;
    struct ast_node_t *node = NULL;
    struct ast_node_t *child = NULL;

    //child = exp_0(tokens);

    token = *tokens;

    //node = child;

    if(token)
    {
        if(token->token_type == TOKEN_PUNCTUATOR)
        {
            switch(token->token_name)
            {
                case TOKEN_PUNCTUATOR_OPARENTHESIS:
                case TOKEN_PUNCTUATOR_OBRACKET:
                case TOKEN_PUNCTUATOR_OBRACE:

                    node = calloc(sizeof(struct ast_node_t), 1);
                    node->token = token;
                    //node->left = child;

                    *tokens = token->next;
                    node->left = exp_16(tokens);
                    token = *tokens;
                    *tokens = token->next;

//                    if(token->token_name != node->token_ + 1)
//                    {
//                        /* error: expecting node->token + 1 at token->token... */
//                    }

                    //node->right = child;
                break;

                case TOKEN_PUNCTUATOR_INCREMENT:
                case TOKEN_PUNCTUATOR_DECREMENT:
                case TOKEN_PUNCTUATOR_DOT:
                case TOKEN_PUNCTUATOR_ARROW:
                    node = calloc(sizeof(struct ast_node_t), 1);
                    node->token = token;
                    node->left = child;

                    *tokens = token->next;
                break;
            }
        }
        else
        {
            switch(token->token_type)
            {
                case TOKEN_CONSTANT:
                case TOKEN_IDENTIFIER:
                    node = calloc(sizeof(struct ast_node_t), 1);
                    node->token = token;
                    *tokens = token->next;
                    node->left = exp_1(tokens);
                break;
            }
        }
    }

    return node;
}

/* literals, identifiers ... */
struct ast_node_t *exp_0(struct token_t **tokens)
{
    struct token_t *token;
    struct ast_node_t *node = NULL;

    token = *tokens;

    if(token)
    {
        switch(token->token_type)
        {
            case TOKEN_IDENTIFIER:
            case TOKEN_CONSTANT:
                node = calloc(sizeof(struct ast_node_t ), 1);
                node->token = token;
                *tokens = token->next;
            break;
        }
    }

    return node;
}



void traverse_ast(struct ast_node_t *ast)
{
    static int depth = -1;
    int i;

    depth++;

    if(ast)
    {
        for(i = 0; i < depth; i++)
        {
            putchar(' ');
        }
        translate_token(ast->token);
        traverse_ast(ast->left);
        traverse_ast(ast->right);
    }
    depth--;
}












