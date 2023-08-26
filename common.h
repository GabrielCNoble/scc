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
    TOKEN_KEYWORD_FIRST = 0,
    TOKEN_KEYWORD_BREAK = TOKEN_KEYWORD_FIRST,
    TOKEN_KEYWORD_CASE,
    
    /********************************/
    TOKEN_KEYWORD_FIRST_TYPE_QUALIFIER,
    TOKEN_KEYWORD_CONST = TOKEN_KEYWORD_FIRST_TYPE_QUALIFIER,
    TOKEN_KEYWORD_RESTRICT,
    TOKEN_KEYWORD_VOLATILE,
    TOKEN_KEYWORD_LAST_TYPE_QUALIFIER = TOKEN_KEYWORD_VOLATILE,
    /********************************/


    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_DEFAULT,
    TOKEN_KEYWORD_DO,
    
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_ENUM,
    
    
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_GOTO,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_INLINE,

    /********************************/
    TOKEN_KEYWORD_FIRST_TYPE_SPECIFIER,
    TOKEN_KEYWORD_INT = TOKEN_KEYWORD_FIRST_TYPE_SPECIFIER,
    TOKEN_KEYWORD_SHORT,
    TOKEN_KEYWORD_CHAR,
    TOKEN_KEYWORD_FLOAT,
    TOKEN_KEYWORD_DOUBLE,
    TOKEN_KEYWORD_LONG,
    TOKEN_KEYWORD_UNSIGNED,
    TOKEN_KEYWORD_SIGNED,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_UNION,
    TOKEN_KEYWORD_LAST_TYPE_SPECIFIER = TOKEN_KEYWORD_UNION,
    /********************************/

    /********************************/
    TOKEN_KEYWORD_FIRST_STORAGE_CLASS_SPECIFIER,
    TOKEN_KEYWORD_TYPEDEF = TOKEN_KEYWORD_FIRST_STORAGE_CLASS_SPECIFIER,
    TOKEN_KEYWORD_EXTERN,
    TOKEN_KEYWORD_STATIC,
    TOKEN_KEYWORD_AUTO,
    TOKEN_KEYWORD_REGISTER,
    TOKEN_KEYWORD_LAST_STORAGE_CLASS_SPECIFIER = TOKEN_KEYWORD_REGISTER,
    /********************************/


    
    
    TOKEN_KEYWORD_RETURN,
    
    
    TOKEN_KEYWORD_SIZEOF,
    
    
    TOKEN_KEYWORD_SWITCH,
    
    
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_UNKNOWN,
    TOKEN_KEYWORD_LAST = TOKEN_KEYWORD_UNKNOWN
};

union constant_t
{
    struct
    {
        char *string_constant;
        uint32_t string_length;
    };
    
    uint64_t int_constant;
    double float_constant;
};

struct token_t
{
    struct token_t *    next;
    union constant_t    constant;
    uint32_t            type;
    uint32_t            name;

    uint32_t            line;
    uint32_t            column;
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
enum DECL_SPEC
{
    DECL_SPEC_INT = 0,
    DECL_SPEC_SHORT,
    DECL_SPEC_CHAR,
    DECL_SPEC_FLOAT,
    DECL_SPEC_DOUBLE,
    DECL_SPEC_LONG,
    DECL_SPEC_UNSIGNED,
    DECL_SPEC_CONST,
    DECL_SPEC_VOLATILE,
    DECL_SPEC_RESTRICT,
    // DECL_SPEC_EXTERN,
    DECL_SPEC_SIGNED,
    DECL_SPEC_VOID,
    DECL_SPEC_STRUCT,
    DECL_SPEC_UNION,
    // DECL_SPEC_POINTER,
    // TYPE_SPECIFIER_TYPEDEF,
    // DECL_SPEC_FUNCTION,
    // DECL_SPEC_ARRAY,
    // DECL_SPEC_LINK,              /* this is only here to make the implementation easier...  */
    // TYPE_SPECIFIER_IDENTIFIER,             /* this is only here to make the implementation easier... */

    DECL_SPEC_TYPEDEF,
    DECL_SPEC_EXTERN,
    DECL_SPEC_STATIC,
    DECL_SPEC_AUTO,
    DECL_SPEC_REGISTER,
    DECL_SPEC_UNKNOWN,
};

#define PRIMITIVE_TYPE_SPEC_MASK ((1 << DECL_SPEC_INT)      |\
                                  (1 << DECL_SPEC_SHORT)    |\
                                  (1 << DECL_SPEC_CHAR)     |\
                                  (1 << DECL_SPEC_FLOAT)    |\
                                  (1 << DECL_SPEC_DOUBLE)   |\
                                  (1 << DECL_SPEC_LONG)     |\
                                  (1 << DECL_SPEC_UNSIGNED) |\
                                  (1 << DECL_SPEC_SIGNED))    

#define AGGREGATE_TYPE_SPEC_MASK ((1 << DECL_SPEC_STRUCT) | (1 << DECL_SPEC_UNION))

#define STORAGE_CLASS_SPEC_MASK ((1 << DECL_SPEC_EXTERN) | (1 << DECL_SPEC_STATIC) | (1 << DECL_SPEC_AUTO) | (1 << DECL_SPEC_REGISTER))

// enum TYPE_QUALIFIER
// {
//     TYPE_QUALIFIER_CONST,
//     TYPE_QUALIFIER_RESTRICT,
//     TYPE_QUALIFIER_VOLATILE,
// };

struct decl_spec_t
{
    struct decl_spec_t *    next;
    uint32_t                specifier;
};

enum TYPES
{
    TYPE_PRIMITIVE,
    TYPE_ARRAY,
    TYPE_POINTER,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_FUNCTION,
    TYPE_UNKNOWN,
};

enum PRIM_TYPES
{
    PRIM_TYPE_CHAR,
    PRIM_TYPE_UCHAR,
    PRIM_TYPE_SHORT,
    PRIM_TYPE_USHORT,
    PRIM_TYPE_INT,
    PRIM_TYPE_UINT,
    PRIM_TYPE_LONG,
    PRIM_TYPE_LLONG,
    PRIM_TYPE_ULONG,
    PRIM_TYPE_ULLONG,
    PRIM_TYPE_FLOAT,
    PRIM_TYPE_DOUBLE,
    PRIM_TYPE_LDOUBLE,
};

struct pointer_t
{
    uint32_t                count;
    struct pointer_t *      next;
    struct decl_spec_t *    qualifiers;
};

/* forward declaration */
struct declarator_t;

struct type_t 
{
    struct type_t *                 next;
    uint32_t                        specifiers;
    struct pointer_t *              pointer;
    struct pointer_t *              last_pointer;

    uint32_t                        type;
    uint32_t                        complete;

    union
    {
        struct 
        { 
            /* used by the parser to store defined types */
            struct type_t *         stored_next;
            /* used for struct/union tags and field names */
            char *                  identifier;
            struct declarator_t *   fields; 
            uint32_t                field_count; 
        } aggregate; 

        struct
        {
            uint32_t                elem_count;
        } array;

        struct
        {
            struct declarator_t *   args;
            uint32_t                arg_count;
            uint32_t                prototype;
            struct scope_t *        body;
        } func;

        struct
        {
            uint32_t                type;
        } primitive;
    };
};

struct declarator_t
{
    struct declarator_t *   next;
    char *                  identifier;
    struct type_t *         type;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum EXP_NODE_TYPE
{
    EXP_NODE_TYPE_PRIMARY = 0,
    EXP_NODE_TYPE_POSTFIX,           
    EXP_NODE_TYPE_UNARY,
    EXP_NODE_TYPE_CAST,
    EXP_NODE_TYPE_MULTIPLICATIVE,
    EXP_NODE_TYPE_ADDITIVE,
    EXP_NODE_TYPE_SHIFT,
    EXP_NODE_TYPE_END,
};

enum PRIMARY_EXP_NODE_TYPE
{
    PRIMARY_EXP_NODE_TYPE_IDENTIFIER = 0,
    PRIMARY_EXP_NODE_TYPE_STRING_LITERAL,
    PRIMARY_EXP_NODE_TYPE_INTEGER_CONSTANT,
    PRIMARY_EXP_NODE_TYPE_EXPRESSION,
};

enum POSTFIX_EXP_NODE_TYPE
{
    POSTFIX_EXP_NODE_TYPE_FUNC_CALL = 0,
    POSTFIX_EXP_NODE_TYPE_ARRAY_INDEX,
    POSTFIX_EXP_NODE_TYPE_INCREMENT,
    POSTFIX_EXP_NODE_TYPE_DECREMENT,
    POSTFIX_EXP_NODE_TYPE_ARROW,
    POSTFIX_EXP_NODE_TYPE_DOT,
};

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

enum MULTIPLICATIVE_EXP_NODE_TYPE
{
    MULTIPLICATIVE_EXP_NODE_TYPE_MULT = 0,
    MULTIPLICATIVE_EXP_NODE_TYPE_DIV,
    MULTIPLICATIVE_EXP_NODE_TYPE_MOD,
};

enum ADDITIVE_EXP_NODE_TYPE
{
    ADDITIVE_EXP_NODE_TYPE_ADD = 0,
    ADDITIVE_EXP_NODE_TYPE_SUB,
};

enum SHIFT_EXP_NODE_TYPE
{
    SHIFT_EXP_NODE_TYPE_SHIFT_LEFT = 0,
    SHIFT_EXP_NODE_TYPE_SHIFT_RIGHT,  
};

struct exp_tree_t
{
    struct exp_tree_t *next;
    struct exp_node_t *root;
};

struct exp_node_t
{
    struct exp_node_t *     left;
    struct exp_node_t *     right;
    uint32_t                type;
    uint32_t                sub_type;
    union constant_t        constant;
};

enum STORAGE_CLASS
{
    STORAGE_CLASS_AUTO = 0,         /* default... */
    STORAGE_CLASS_EXTERN,
    STORAGE_CLASS_STATIC,
};

struct object_t
{
    struct object_t *       next;
    struct scope_t *        scope;
    struct declarator_t *   declarator;
    // struct type_t *     type;
    // struct base_type_t *type;
    // char *              identifier;
    // int                 storage_class;
    unsigned int            offset;            /* offset from the segment pointer... */
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

struct base_statement_t
{
    struct base_statement_t *next;
    uint32_t type;
};

struct block_statement_t
{
    struct base_statement_t base;
    struct base_exp_node_t *exps;
};

struct if_statement_t
{
    struct base_statement_t base;
    struct base_exp_node_t *condition;
};

struct for_statement_t
{
    struct base_statement_t base;
    struct base_exp_node_t *first_init_exp;
    struct base_exp_node_t *middle_init_exp;
    struct base_exp_node_t *last_init_exp;
};

struct switch_statement_t
{
    struct base_statement_t base;
    struct base_exp_node_t *switch_exp;
};

/*
************************************************************
************************************************************
************************************************************
*/

struct scope_t
{
    struct scope_t *        parent;
    struct scope_t *        children;
    struct scope_t *        last_child;
    struct scope_t *        next;

    struct object_t *       objects;
    struct object_t *       last_object;

    struct exp_tree_t *     expressions;
    struct exp_tree_t *     last_expression;

    /* whose function this is the body of */
    struct declarator_t *   function;
    uint32_t                active;
};

/*
************************************************************
************************************************************
************************************************************
*/

enum PARSER_FLAGS
{
    PARSER_FLAG_ARG_LIST = 1,
    PARSER_FLAG_TYPE_NAME = 1 << 1,
    // PARSER_FLAG_NO_INIT = 1 << 2
};

struct parser_t
{
    // struct token_t *            tokens;
    // struct token_t *            cur_token;
    // struct token_t *            next_token;

    uint32_t                    line;
    uint32_t                    column;
    uint32_t                    src_offset;
    char *                      src;

    struct token_t              cur_token;
    struct token_t              next_token;

    uint32_t                    declaration_depth;

    uint32_t                    param_list_level;
    uint32_t                    aggregate_level;

    struct type_t *             aggregate_types;
    struct base_type_t *        typedef_types;

//    int scope_stack_top;
//    struct scope_t **scope_stack;
    struct scope_t *            cur_scope;
    struct declarator_t *       cur_function;
};

#endif // COMMON_H
