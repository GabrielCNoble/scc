#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum CHAR_TYPE
{
    CHAR_PUNCTUATOR = 1,
    CHAR_LETTER = 1 << 1,
    CHAR_NUMBER = 1 << 2,
    CHAR_SPACE = 1 << 3,
    CHAR_UNKNOWN = 1 << 4,
};

enum TOKEN
{
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING_LITERAL,
    TOKEN_PUNCTUATOR,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
};


enum TOKEN_CONSTANT
{
    TOKEN_CONSTANT_INTEGER,
    TOKEN_CONSTANT_FLOAT,
    TOKEN_CONSTANT_CHAR,
    TOKEN_CONSTANT_HEX,
};


enum TOKEN_PUNCTUATOR
{
    TOKEN_PUNCTUATOR_PLUS = 0,
    TOKEN_PUNCTUATOR_INCREMENT,
    TOKEN_PUNCTUATOR_PLUS_ASSIGN,
    TOKEN_PUNCTUATOR_MINUS,
    TOKEN_PUNCTUATOR_DECREMENT,
    TOKEN_PUNCTUATOR_MINUS_ASSIGN,
    TOKEN_PUNCTUATOR_ASTERISC,
    TOKEN_PUNCTUATOR_MUL_ASSIGN,
    TOKEN_PUNCTUATOR_SLASH,
    TOKEN_PUNCTUATOR_DIV_ASSIGN,
    TOKEN_PUNCTUATOR_PERCENT,
    TOKEN_PUNCTUATOR_MOD_ASSIGN,
    TOKEN_PUNCTUATOR_AMPERSAND,
    TOKEN_PUNCTUATOR_BW_AND_ASSIGN,
    TOKEN_PUNCTUATOR_LOG_AND,
    TOKEN_PUNCTUATOR_EXCLAMATION,
    TOKEN_PUNCTUATOR_NOT_EQUAL,
    TOKEN_PUNCTUATOR_ASSIGN,
    TOKEN_PUNCTUATOR_EQUAL,
    TOKEN_PUNCTUATOR_XOR,
    TOKEN_PUNCTUATOR_BW_XOR_ASSIGN,
    TOKEN_PUNCTUATOR_BW_OR,
    TOKEN_PUNCTUATOR_BW_OR_ASSIGN,
    TOKEN_PUNCTUATOR_LOG_OR,
    TOKEN_PUNCTUATOR_TILDE,
    TOKEN_PUNCTUATOR_LESS_THAN,
    TOKEN_PUNCTUATOR_LESS_THAN_OR_EQUAL,
    TOKEN_PUNCTUATOR_LEFT_SHIFT,
    TOKEN_PUNCTUATOR_LEFT_SHIFT_ASSIGN,
    TOKEN_PUNCTUATOR_GREATER_THAN,
    TOKEN_PUNCTUATOR_GREATER_THAN_OR_EQUAL,
    TOKEN_PUNCTUATOR_RIGHT_SHIFT,
    TOKEN_PUNCTUATOR_RIGHT_SHIFT_ASSIGN,
    TOKEN_PUNCTUATOR_OPARENTHESIS,
    TOKEN_PUNCTUATOR_CPARENTHESIS,
    TOKEN_PUNCTUATOR_OBRACE,
    TOKEN_PUNCTUATOR_CBRACE,
    TOKEN_PUNCTUATOR_OBRACKET,
    TOKEN_PUNCTUATOR_CBRACKET,
    TOKEN_PUNCTUATOR_COLON,
    TOKEN_PUNCTUATOR_SEMICOLON,
    TOKEN_PUNCTUATOR_COMMA,
    TOKEN_PUNCTUATOR_DOT,
    TOKEN_PUNCTUATOR_ARROW,
    TOKEN_PUNCTUATOR_UNKNOWN,
};


enum TOKEN_KEYWORD
{
    TOKEN_KEYWORD_AUTO = 0,
    TOKEN_KEYWORD_BREAK,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_CHAR,
    TOKEN_KEYWORD_CONST,
    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_DEFAULT,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_DOUBLE,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_ENUM,
    TOKEN_KEYWORD_EXTERN,
    TOKEN_KEYWORD_FLOAT,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_GOTO,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_INLINE,
    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_LONG,
    TOKEN_KEYWORD_REGISTER,
    TOKEN_KEYWORD_RESTRICT,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_SHORT,
    TOKEN_KEYWORD_SIGNED,
    TOKEN_KEYWORD_SIZEOF,
    TOKEN_KEYWORD_STATIC,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_SWITCH,
    TOKEN_KEYWORD_TYPEDEF,
    TOKEN_KEYWORD_UNION,
    TOKEN_KEYWORD_UNSIGNED,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_VOLATILE,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_UNKNOWN,
};

union constant_t
{
    char *string_constant;
    uint64_t int_constant;
    double float_constant;
};

struct token_t
{
    struct token_t *next;
    union constant_t constant;
    int token_type;
    int token_name;

    unsigned int line;
    unsigned int column;
//    int token;
//    int reserved_token;
};


/*
************************************************************
************************************************************
************************************************************
*/


/* not everything here is a type specifier, but this makes the parser
implementation simpler, and also allow for easy type comparison... */
enum TYPE
{
    TYPE_INT = 0,
    TYPE_SHORT,
    TYPE_CHAR,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG,
    TYPE_UNSIGNED,
    TYPE_CONST,
    TYPE_VOLATILE,
    TYPE_RESTRICT,
    TYPE_EXTERN,
    TYPE_SIGNED,
    TYPE_VOID,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_POINTER,
    TYPE_TYPEDEF,
    TYPE_FUNCTION,
    TYPE_ARRAY,
    TYPE_LINK,              /* this is only here to make the implementation easier...  */
    TYPE_IDENTIFIER,             /* this is only here to make the implementation easier... */
    TYPE_UNKNOWN,
};

struct base_type_t
{
    struct base_type_t *next;
    unsigned short type;
};

struct array_type_t
{
    struct base_type_t base;
    struct base_type_t *base_type;
    int size;
};

struct aggretage_type_t
{
    struct base_type_t base;
    struct link_type_t *fields;
    char *name;
};

struct function_type_t
{
    struct base_type_t base;
    struct link_type_t *args;
    int arg_count;
    int old_style;
};

/* not really a type, but is here
to make implementation easier... */
struct identifier_type_t
{
    struct base_type_t base;
    char *identifier;
};

/* used to link types together, as in
the fields of a struct/union */
struct link_type_t
{
    struct base_type_t base;
    struct base_type_t *type;
};

/* not really a type, but is here
to make implementation easier... */
struct typedef_type_t
{
    struct base_type_t base;
    struct base_type_t *type;
};


/*
************************************************************
************************************************************
************************************************************
*/



enum EXP_NODE_TYPE
{
    EXP_NODE_TYPE_PRIMARY = 1,
    EXP_NODE_TYPE_POSTFIX = 1 << 1,
    EXP_NODE_TYPE_UNARY = 1 << 2,
    EXP_NODE_TYPE_MULTIPLICATIVE = 1 << 3,
    EXP_NODE_TYPE_ADDITIVE = 1 << 4,
    EXP_NODE_TYPE_END,
};

struct base_exp_node_t
{
    struct base_exp_node_t *left;
    struct base_exp_node_t *right;
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
    PRIMARY_EXP_NODE_TYPE_EXPRESSION,
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
    MULTIPLICATIVE_EXP_NODE_TYPE_MOD,
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

enum STORAGE_CLASS
{
    STORAGE_CLASS_AUTO = 0,         /* default... */
    STORAGE_CLASS_EXTERN,
    STORAGE_CLASS_STATIC,
};

struct object_t
{
    struct object_t *next;
    struct scope_t *scope;
    struct base_type_t *type;
    char *id;
    int storage_class;
    unsigned int offset;            /* offset from the segment pointer... */
};

struct function_t
{
    struct object_t *object;
};

/*
************************************************************
************************************************************
************************************************************
*/

struct scope_t
{
    struct scope_t *parent;
    struct object_t *objects;
    struct object_t *last_object;
    uint32_t active;
};

/*
************************************************************
************************************************************
************************************************************
*/


struct parser_t
{
    struct token_t *tokens;
    struct token_t *current_token;
    struct token_t *next_token;

    int declaration_depth;

    int param_list_level;
    int aggregate_level;

    struct aggretage_type_t *aggregate_types;
    struct base_type_t *typedef_types;

//    int scope_stack_top;
//    struct scope_t **scope_stack;
    struct scope_t *current_scope;
};

#endif // COMMON_H
