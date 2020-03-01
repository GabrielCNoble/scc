#ifndef EXP_H
#define EXP_H


#include "tok.h"
#include "parse.h"
#include "obj.h"
#include "type.h"
#include <stdint.h>



enum EXP_NODE_TYPE
{
    EXP_NODE_TYPE_PRIMARY = 0,
    EXP_NODE_TYPE_POSTFIX,
    EXP_NODE_TYPE_UNARY,
    EXP_NODE_TYPE_MULTIPLICATIVE,
    EXP_NODE_TYPE_ADDITIVE,
    EXP_NODE_TYPE_END,
};

struct base_exp_node_t
{
    struct base_exp_node_t *next;
    uint16_t type;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum PRIMARY_EXP_NODE_TYPE
{
    PRIMARY_EXP_NODE_TYPE_IDENTIFIER = 0,
    PRIMARY_EXP_NODE_TYPE_STRING_LITERAL,
    PRIMARY_EXP_NODE_TYPE_INTEGER_CONSTANT,
};

struct primary_exp_node_t
{
    struct base_exp_node_t base;
    uint16_t type;
    union constant_t constant;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum POSTFIX_EXP_NODE_TYPE
{
    POSTFIX_EXP_NODE_TYPE_FUNC_CALL = 0,
    POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX,
    POSTFIX_EXP_NODE_TYPE_INCREMENT,
    POSTFIX_EXP_NODE_TYPE_DECREMENT,
    POSTFIX_EXP_NODE_TYPE_ARROW,
    POSTFIX_EXP_NODE_TYPE_DOT,
};

struct postfix_exp_node_t
{
    struct base_exp_node_t base;
    uint16_t type;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum UNARY_EXP_NODE_TYPE
{
    UNARY_EXP_NODE_TYPE_INCREMENT,
    UNARY_EXP_NODE_TYPE_DECREMENT,
    UNARY_EXP_NODE_TYPE_ADDRESS_OF,
    UNARY_EXP_NODE_TYPE_DEREFERENCE,
    UNARY_EXP_NODE_TYPE_PLUS,
    UNARY_EXP_NODE_TYPE_MINUS,
    UNARY_EXP_NODE_TYPE_BITWISE_NOT,
    UNARY_EXP_NODE_TYPE_LOGICAL_NOT,
    UNARY_EXP_NODE_TYPE_SIZEOF
};

struct unary_exp_node_t
{
    struct base_exp_node_t base;
    uint16_t type;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum MULTIPLICATIVE_EXP_NODE_TYPE
{
    MULTIPLICATIVE_EXP_NODE_TYPE_MULT = 0,
    MULTIPLICATIVE_EXP_NODE_TYPE_DIV,
    MULTIPLICATIVE_EXP_NODE_TYPE_MOD
};

struct multiplicative_exp_node_t
{
    struct base_exp_node_t base;
    uint16_t type;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum ADDITIVE_EXP_NODE_TYPE
{
    ADDITIVE_EXP_NODE_TYPE_ADD = 0,
    ADDITIVE_EXP_NODE_TYPE_SUB,
};

struct additive_exp_node_t
{
    struct base_exp_node_t base;
    uint16_t type;
};

/*
************************************************************
************************************************************
************************************************************
*/


enum ARITHMETIC_EXP_NODE_TYPE
{
    ARITHMETIC_EXP_NODE_TYPE_ADDITION = 0,
    ARITHMETIC_EXP_NODE_TYPE_SUBTRACTION,
    ARITHMETIC_EXP_NODE_TYPE_MULTIPLICATION,
    ARITHMETIC_EXP_NODE_TYPE_DIVISION,
    ARITHMETIC_EXP_NODE_TYPE_MODULO,
    ARITHMETIC_EXP_NODE_TYPE_PRE_DECREMENT,
    ARITHMETIC_EXP_NODE_TYPE_POST_DECREMENT,
};

struct arithmetic_exp_node_t
{
    struct base_exp_node_t base;
    uint16_t type;
};




struct base_exp_node_t *exp(struct parser_t *parser);

void comma_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void assignment_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void conditional_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void logical_or_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void logical_and_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void or_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void xor_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void and_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void equality_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void relational_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void shift_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void additive_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void multiplicative_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void cast_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void unary_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void postfix_exp(struct parser_t *parser, struct base_exp_node_t ***cur);

void primary_exp(struct parser_t *parser, struct base_exp_node_t ***cur);



#endif // EXP_H








