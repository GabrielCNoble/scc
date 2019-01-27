#ifndef TOKEN_H
#define TOKEN_H

//enum CHAR
//{
//    CHAR_PLUS = 0,
//    CHAR_MINUS,
//    CHAR_BAR,
//    CHAR_ASTERISC,
//    CHAR_NUMBER,
//    CHAR_LETTER,
//    CHAR_SEMICOLON,
//    CHAR_OPARENTHESIS,
//    CHAR_CPARENTHESIS,
//    CHAR_EQUALS,
//    CHAR_EXCLAMATION,
//    CHAR_SPACE,
//    CHAR_COMMA,
//    CHAR_UNKNOWN,
//};

enum CHAR_TYPE
{
    CHAR_SEPARATOR = 1,
    CHAR_LETTER = 1 << 1,
    CHAR_NUMBER = 1 << 2,
    CHAR_SPACE = 1 << 3,
    CHAR_UNKNOWN = 1 << 4,
};

enum TOKEN
{
    TOKEN_PLUS = 0,
    TOKEN_PLUS_ASSIGN,
    TOKEN_INCREMENT,

    TOKEN_MINUS,
    TOKEN_MINUS_ASSIGN,
    TOKEN_DECREMENT,

    TOKEN_DIV,
    TOKEN_DIV_ASSIGN,

    TOKEN_ASTERISC,
    TOKEN_MUL_ASSIGN,

    TOKEN_NUMBER,
    TOKEN_ID,

    TOKEN_RESERVED,

    TOKEN_COMMA,

    TOKEN_ASSIGN,
    TOKEN_EQUALS,
    TOKEN_NOT_EQUALS,
    TOKEN_LOGICAL_NOT,


    TOKEN_SEMICOLON,
    TOKEN_OPARENTHESIS,
    TOKEN_CPARENTHESIS,
    TOKEN_UNKNOWN,
};


enum RESERVED_TOKEN
{
    RESERVED_TOKEN_FOR,
    RESERVED_TOKEN_IF,
    RESERVED_TOKEN_ELSE,
    RESERVED_TOKEN_WHILE,
    RESERVED_TOKEN_DO,
    RESERVED_TOKEN_VOID,
    RESERVED_TOKEN_INT,
    RESERVED_TOKEN_SHORT,
    RESERVED_TOKEN_CHAR,
    RESERVED_TOKEN_UNKNOWN,
};


struct token_t
{
    struct token_t *next;
    char text[64];
    int token;
    int reserved_token;
};


void token_init();

struct token_t *tokenize(char *text);

void free_tokens(struct token_t *tokens);

void translate_tokens(struct token_t *token);




#endif // TOKEN_H








