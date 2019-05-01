#ifndef EXP_H
#define EXP_H


#include "tok.h"
#include "parse.h"
#include "obj.h"
#include "type.h"

enum EXP_RESULT_TYPE
{
    EXP_RESULT_TYPE_LITERAL,
    EXP_RESULT_TYPE_OBJECT,
    EXP_RESULT_TYPE_ERROR,
    EXP_RESULT_TYPE_UNKNOWN,
};




union exp_result_value_t
{
    struct object_t *object_result;
    int int_result;
    short short_result;
    float float_result;
    double double_result;
    void *pointer_result;
};

//union exp_result_value_t
//{
//    struct object_t *object;
//    int int_result;
//};

struct exp_result_t
{
    int immediate;
    struct base_type_t *type;
    void *value;
};

//struct ast_node_t
//{
//    struct ast_node_t *left;
//    struct ast_node_t *right;
//    struct token_t *token;
//};


//struct token_t *get_token();

//void advance_token();

//void expression(struct token_t *tokens);
//
//void expression0(struct exp_result_t *result, struct token_t **tokens);
//
//void expression1(struct exp_result_t *result, struct token_t **tokens);
//
//void expression2(struct exp_result_t *result, struct token_t **tokens);
//
//void expression3(struct exp_result_t *result, struct token_t **tokens);
//
//void expression4(struct exp_result_t *result, struct token_t **tokens);
//
//void expression5(struct exp_result_t *result, struct token_t **tokens);
//
//void expression6(struct exp_result_t *result, struct token_t **tokens);
//
//void expression7(struct exp_result_t *result, struct token_t **tokens);




struct exp_result_t exp_16(struct parser_t *parser);

struct exp_result_t exp_15(struct parser_t *parser);

struct exp_result_t exp_14(struct parser_t *parser);

struct exp_result_t exp_13(struct parser_t *parser);

struct exp_result_t exp_12(struct parser_t *parser);

struct exp_result_t exp_11(struct parser_t *parser);

struct exp_result_t exp_10(struct parser_t *parser);

struct exp_result_t exp_9(struct parser_t *parser);

struct exp_result_t exp_8(struct parser_t *parser);

struct exp_result_t exp_7(struct parser_t *parser);

struct exp_result_t exp_6(struct parser_t *parser);

struct exp_result_t exp_5(struct parser_t *parser);

struct exp_result_t exp_4(struct parser_t *parser);

struct exp_result_t exp_3(struct parser_t *parser);

struct exp_result_t exp_2(struct parser_t *parser);

struct exp_result_t exp_1(struct parser_t *parser);

struct exp_result_t exp_0(struct parser_t *parser);


//void traverse_ast(struct ast_node_t *ast);


#endif // EXP_H








