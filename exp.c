#include "exp.h"
#include "var.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static struct token_t *next_token = NULL;


struct token_t *get_token()
{
    return next_token;
}

void advance_token()
{
    if(next_token)
    {
        next_token = next_token->next;
    }
}

//void parse_statement(struct token_t *tokens)
void parse_statement(char *statement)
{
    struct token_t *tokens;
    struct token_t *token;
    struct exp_result_t result;

    int success = 1;

    tokens = tokenize(statement);

    if(!next_token)
    {
        next_token = tokens;
    }

    do
    {
        expression6(&result);
        token = get_token();

        if((!token) || token->token != TOKEN_SEMICOLON)
        {
            printf("yo fat fuck, no semicolon at the end of the statement!\n");
            success = 0;
        }
        advance_token();
        token = get_token();

    }while(token && token->token == TOKEN_SEMICOLON);

    if(success)
    {
        if(result.result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            printf("-->%d\n", result.result_value.var->value.ivalue);
        }
        else if(result.result_type == EXP_RESULT_TYPE_LITERAL)
        {
            printf("-->%d\n", result.result_value.int_result);
        }
        else
        {
            printf("-->%s\n", result.text);
        }
    }

    free_tokens(tokens);
}
//
///* deals with '+, -' */
//void exp_1(int *result)
//{
//    struct token_t *token;
//
//    int temp = 0;
//
//    exp_2(result);
//
//    token = get_token();
//
//    while(token && (token->token == TOKEN_PLUS || token->token == TOKEN_MINUS))
//    {
//        advance_token();
//
//        temp = 0;
//
//        exp_2(&temp);
//
//        switch(token->token)
//        {
//            case TOKEN_PLUS:
//                (*result) += temp;
//            break;
//
//            case TOKEN_MINUS:
//                (*result) -= temp;
//            break;
//        }
//
//        token = get_token();
//    }
//}
//
///* deals with '/, *' */
//void exp_2(int *result)
//{
//    struct token_t *token;
//
//    int temp;
//
//    exp_3(result);
//
//    token = get_token();
//
//    while(token && (token->token == TOKEN_MUL || token->token == TOKEN_DIV))
//    {
//        advance_token();
//
//        temp = 0;
//
//        exp_3(&temp);
//
//        switch(token->token)
//        {
//            case TOKEN_MUL:
//                (*result) *= temp;
//            break;
//
//            case TOKEN_DIV:
//                (*result) /= temp;
//            break;
//        }
//
//        token = get_token();
//    }
//}
//
///* unary - and + */
//void exp_3(int *result)
//{
//    struct token_t *token;
//
//    token = get_token();
//
//    int token_type = TOKEN_UNKNOWN;
//
//    if(token)
//    {
//        while(token->token == TOKEN_PLUS || token->token == TOKEN_MINUS)
//        {
//            switch(token->token)
//            {
//                case TOKEN_MINUS:
//                    token_type = token_type == TOKEN_MINUS ? TOKEN_UNKNOWN : TOKEN_MINUS;
//                break;
//            }
//
//            advance_token();
//            token = get_token();
//        }
//    }
//
//    exp_4(result);
//
//    switch(token_type)
//    {
//        case TOKEN_MINUS:
//            *result = -*result;
//        break;
//    }
//}
//
///* parenthesis... */
//void exp_4(int *result)
//{
//    struct token_t *token;
//
//    token = get_token();
//    advance_token();
//
//    if(token->token == TOKEN_OPARENTHESIS)
//    {
//        exp_1(result);
//        advance_token();
//    }
//    else
//    {
//        *result = atoi(token->text);
//    }
//}


//void parse_statement(char *statement)
//{
//    struct token_t *token;
//    struct exp_result_t *result;
//
//    tokenize(statement);
//
//    token = get_token();
//
//    while(token && token->token == TOKEN_SEMICOLON)
//    {
//        expression6()
//
//        token = get_token();
//    }
//}

/* assignments... */
void expression6(struct exp_result_t *result)
{
    struct token_t *token;
//    struct var_t *var;
//    struct var_t *temp_var;

    struct exp_result_t temp;
    int temp_value;

    char error_buffer[128];

    expression5(result);

    token = get_token();

    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_ASSIGN || token->token == TOKEN_PLUS_ASSIGN ||
                                                                                        token->token == TOKEN_MINUS_ASSIGN ||
                                                                                        token->token == TOKEN_MUL_ASSIGN ||
                                                                                        token->token == TOKEN_DIV_ASSIGN)))
    {
        advance_token();
        expression5(&temp);

        /* left side exp... */
        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!result->result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(error_buffer, "undefined identifier [%s]", result->text);
                strcpy(result->text, error_buffer);
                return;
            }
        }
        else if(result->result_type == EXP_RESULT_TYPE_LITERAL)
        {
            result->result_type = EXP_RESULT_TYPE_ERROR;
            sprintf(result->text, "literal cannot be lvalue");
            return;
        }


        /* right exp... */
        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!temp.result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(result->text, "undefined identifier [%s]", temp.text);
                return;
            }
            else
            {
                temp_value = temp.result_value.var->value.ivalue;
            }
        }
        else
        {
            temp_value = temp.result_value.int_result;
        }

        switch(token->token)
        {
            case TOKEN_ASSIGN:
                result->result_value.var->value.ivalue = temp_value;
            break;

            case TOKEN_PLUS_ASSIGN:
                result->result_value.var->value.ivalue += temp_value;
            break;

            case TOKEN_MINUS_ASSIGN:
                result->result_value.var->value.ivalue -= temp_value;
            break;

            case TOKEN_MUL_ASSIGN:
                result->result_value.var->value.ivalue *= temp_value;
            break;

            case TOKEN_DIV_ASSIGN:
                result->result_value.var->value.ivalue /= temp_value;
            break;
        }

        result->result_type = EXP_RESULT_TYPE_LITERAL;
        result->result_value.int_result = result->result_value.var->value.ivalue;

        token = get_token();
    }
}

/* var decls... */
void expression5(struct exp_result_t *result)
{
    struct token_t *token;
    struct var_t *var;
    struct exp_result_t temp_result;

    int var_type;

    expression4(result);

    token = get_token();

    if(token && (token->token != TOKEN_SEMICOLON && token->token == TOKEN_RESERVED))
    {
        advance_token();

        expression4(&temp_result);

        switch(temp_result.result_type)
        {
            case EXP_RESULT_TYPE_VARIABLE:
                if(!temp_result.result_value.var)
                {
                    /* var declaration... */

                    switch(token->reserved_token)
                    {
                        case RESERVED_TOKEN_INT:
                            var_type = VAR_TYPE_INT;
                        break;
                    }

                    create_var(temp_result.text, var_type);

                    var = get_var(temp_result.text);

                    result->result_type = EXP_RESULT_TYPE_VARIABLE;
                    result->result_value.var = var;
                    strcpy(result->text, temp_result.text);
                }
                else
                {
                    result->result_type = EXP_RESULT_TYPE_ERROR;
                    sprintf(result->text, "redeclaration of identifier [%s]", temp_result.text);
                }
            break;

            case EXP_RESULT_TYPE_LITERAL:
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(result->text, "identifier starting with number");
            break;

        }

        return;
    }
}

/* == */
void expression4(struct exp_result_t *result)
{
    struct token_t *token;

    struct exp_result_t temp;
    char error_buffer[128];

    int result_value;
    int temp_value;

    expression3(result);

    token = get_token();

    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_EQUALS || token->token == TOKEN_NOT_EQUALS)))
    {
        advance_token();
        expression3(&temp);


        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!result->result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(error_buffer, "undefined identifier [%s]", result->text);
                strcpy(result->text, error_buffer);
            }
            else
            {
                result_value = result->result_value.var->value.ivalue;
            }
        }
        else
        {
            result_value = result->result_value.int_result;
        }



        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!temp.result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(result->text, "undefined identifier [%s]", temp.text);
            }
            else
            {
                temp_value = temp.result_value.var->value.ivalue;
            }
        }
        else
        {
            temp_value = temp.result_value.int_result;
        }



        switch(token->token)
        {
            case TOKEN_EQUALS:
                result_value = result_value == temp_value;
            break;

            case TOKEN_NOT_EQUALS:
                result_value = result_value != temp_value;
            break;
        }

        result->result_type = EXP_RESULT_TYPE_LITERAL;
        result->result_value.int_result = result_value;

        token = get_token();
    }
}

/* +- */
void expression3(struct exp_result_t *result)
{
    struct token_t *token;
    struct exp_result_t temp;

    char error_buffer[128];

    expression2(result);

    int result_value;
    int temp_value;

    token = get_token();

    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_PLUS || token->token == TOKEN_MINUS)))
    {
        advance_token();
        expression2(&temp);



        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!result->result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(error_buffer, "undefined identifier [%s]", result->text);
                strcpy(result->text, error_buffer);
            }
            else
            {
                result_value = result->result_value.var->value.ivalue;
            }
        }
        else
        {
            result_value = result->result_value.int_result;
        }



        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!temp.result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(result->text, "undefined identifier [%s]", temp.text);
            }
            else
            {
                temp_value = temp.result_value.var->value.ivalue;
            }
        }
        else
        {
            temp_value = temp.result_value.int_result;
        }

        switch(token->token)
        {
            case TOKEN_PLUS:
                result_value += temp_value;
            break;

            case TOKEN_MINUS:
                result_value -= temp_value;
            break;
        }

        result->result_type = EXP_RESULT_TYPE_LITERAL;
        result->result_value.int_result = result_value;

        token = get_token();
    }
}

/* / * */
void expression2(struct exp_result_t *result)
{
    struct token_t *token;
    struct exp_result_t temp;

    char error_buffer[128];

    int result_value;
    int temp_value;

    expression1(result);

    token = get_token();

    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_ASTERISC || token->token == TOKEN_DIV)))
    {
        advance_token();
        expression1(&temp);



        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!result->result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(error_buffer, "undefined identifier [%s]", result->text);
                strcpy(result->text, error_buffer);
            }
            else
            {
                result_value = result->result_value.var->value.ivalue;
            }
        }
        else
        {
            result_value = result->result_value.int_result;
        }



        if(temp.result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!temp.result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(result->text, "undefined identifier [%s]", temp.text);
            }
            else
            {
                temp_value = temp.result_value.var->value.ivalue;
            }
        }
        else
        {
            temp_value = temp.result_value.int_result;
        }

        switch(token->token)
        {
            case TOKEN_ASTERISC:
                result_value *= temp_value;
            break;

            case TOKEN_DIV:
                result_value /= temp_value;
            break;
        }

        result->result_type = EXP_RESULT_TYPE_LITERAL;
        result->result_value.int_result = result_value;

        token = get_token();
    }
}

/* unary +- */
void expression1(struct exp_result_t *result)
{
    struct token_t *token;
    int result_value;

    char error_buffer[128];

    int token_type = TOKEN_UNKNOWN;

    token = get_token();

    while(token && (token->token != TOKEN_SEMICOLON && (token->token == TOKEN_PLUS || token->token == TOKEN_MINUS)))
    {
        token_type = token_type == TOKEN_MINUS ? TOKEN_UNKNOWN : TOKEN_MINUS;

        advance_token();
        token = get_token();
    }

    expression0(result);

    if(token_type == TOKEN_MINUS)
    {
        if(result->result_type == EXP_RESULT_TYPE_VARIABLE)
        {
            if(!result->result_value.var)
            {
                result->result_type = EXP_RESULT_TYPE_ERROR;
                sprintf(error_buffer, "undefined identifier [%s]", result->text);
                strcpy(result->text, error_buffer);
                return;
            }
            else
            {
                result_value = result->result_value.var->value.ivalue;
            }
        }
        else
        {
            result_value = result->result_value.int_result;
        }

        result_value = -result_value;
        result->result_type = EXP_RESULT_TYPE_LITERAL;
        result->result_value.int_result = result_value;
    }
}

/* () number... */
void expression0(struct exp_result_t *result)
{
    struct token_t *token;
    struct var_t *var;

    token = get_token();

    if(token && token->token != TOKEN_SEMICOLON)
    {
        if(token->token == TOKEN_OPARENTHESIS)
        {
            advance_token();
            expression6(result);
            advance_token();
        }
        else
        {
            if(token->token == TOKEN_NUMBER)
            {
                result->result_type = EXP_RESULT_TYPE_LITERAL;
                result->result_value.int_result = atoi(token->text);
                advance_token();
            }
            else if(token->token == TOKEN_ID)
            {
                var = get_var(token->text);

                result->result_type = EXP_RESULT_TYPE_VARIABLE;
                result->result_value.var = var;
                strcpy(result->text, token->text);
                advance_token();
            }
        }
    }
}














