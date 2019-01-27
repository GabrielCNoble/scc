#ifndef EXP_H
#define EXP_H


#include "token.h"
#include "var.h"

enum EXP_RESULT_TYPE
{
    EXP_RESULT_TYPE_LITERAL,
    EXP_RESULT_TYPE_VARIABLE,
    EXP_RESULT_TYPE_ERROR,
    EXP_RESULT_TYPE_UNKNOWN,
};

union exp_result_value_t
{
    struct var_t *var;
    int int_result;
};

struct exp_result_t
{
    int result_type;
    union exp_result_value_t result_value;
    char text[128];
};


struct token_t *get_token();

void advance_token();

//void exp_start(struct token_t *tokens);
//
//void exp_0(int *result);
//
//void exp_1(int *result);
//
//void exp_2(int *result);
//
//void exp_3(int *result);
//
//void exp_4(int *result);


void parse_statement(char *statement);

void expression0(struct exp_result_t *result);

void expression1(struct exp_result_t *result);

void expression2(struct exp_result_t *result);

void expression3(struct exp_result_t *result);

void expression4(struct exp_result_t *result);

void expression5(struct exp_result_t *result);

void expression6(struct exp_result_t *result);




#endif // EXP_H
