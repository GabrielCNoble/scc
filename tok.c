#include "tok.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static char char_map[255] = {
    ['0'] = CHAR_NUMBER,
    ['1'] = CHAR_NUMBER,
    ['2'] = CHAR_NUMBER,
    ['3'] = CHAR_NUMBER,
    ['4'] = CHAR_NUMBER,
    ['5'] = CHAR_NUMBER,
    ['6'] = CHAR_NUMBER,
    ['7'] = CHAR_NUMBER,
    ['8'] = CHAR_NUMBER,
    ['9'] = CHAR_NUMBER,

    ['a'] = CHAR_LETTER,
    ['A'] = CHAR_LETTER,
    ['b'] = CHAR_LETTER,
    ['B'] = CHAR_LETTER,
    ['c'] = CHAR_LETTER,
    ['C'] = CHAR_LETTER,
    ['d'] = CHAR_LETTER,
    ['D'] = CHAR_LETTER,
    ['e'] = CHAR_LETTER,
    ['E'] = CHAR_LETTER,
    ['f'] = CHAR_LETTER,
    ['F'] = CHAR_LETTER,
    ['g'] = CHAR_LETTER,
    ['G'] = CHAR_LETTER,
    ['h'] = CHAR_LETTER,
    ['H'] = CHAR_LETTER,
    ['i'] = CHAR_LETTER,
    ['I'] = CHAR_LETTER,
    ['j'] = CHAR_LETTER,
    ['J'] = CHAR_LETTER,
    ['k'] = CHAR_LETTER,
    ['K'] = CHAR_LETTER,
    ['l'] = CHAR_LETTER,
    ['L'] = CHAR_LETTER,
    ['m'] = CHAR_LETTER,
    ['M'] = CHAR_LETTER,
    ['n'] = CHAR_LETTER,
    ['N'] = CHAR_LETTER,
    ['o'] = CHAR_LETTER,
    ['O'] = CHAR_LETTER,
    ['p'] = CHAR_LETTER,
    ['P'] = CHAR_LETTER,
    ['q'] = CHAR_LETTER,
    ['Q'] = CHAR_LETTER,
    ['r'] = CHAR_LETTER,
    ['R'] = CHAR_LETTER,
    ['s'] = CHAR_LETTER,
    ['S'] = CHAR_LETTER,
    ['t'] = CHAR_LETTER,
    ['T'] = CHAR_LETTER,
    ['u'] = CHAR_LETTER,
    ['U'] = CHAR_LETTER,
    ['v'] = CHAR_LETTER,
    ['V'] = CHAR_LETTER,
    ['w'] = CHAR_LETTER,
    ['W'] = CHAR_LETTER,
    ['x'] = CHAR_LETTER,
    ['X'] = CHAR_LETTER,
    ['y'] = CHAR_LETTER,
    ['Y'] = CHAR_LETTER,
    ['z'] = CHAR_LETTER,
    ['Z'] = CHAR_LETTER,
    ['_'] = CHAR_LETTER,

    ['\0']  = CHAR_SPACE,
    [' ']   = CHAR_SPACE,
    ['\n']  = CHAR_SPACE,

    ['+'] = CHAR_PUNCTUATOR,
    ['-'] = CHAR_PUNCTUATOR,
    ['/'] = CHAR_PUNCTUATOR,
    ['*'] = CHAR_PUNCTUATOR,
    ['%'] = CHAR_PUNCTUATOR,
    [';'] = CHAR_PUNCTUATOR,
    [':'] = CHAR_PUNCTUATOR,
    ['('] = CHAR_PUNCTUATOR,
    [')'] = CHAR_PUNCTUATOR,
    [','] = CHAR_PUNCTUATOR,
    ['='] = CHAR_PUNCTUATOR,
    ['!'] = CHAR_PUNCTUATOR,
    ['~'] = CHAR_PUNCTUATOR,
    ['{'] = CHAR_PUNCTUATOR,
    ['}'] = CHAR_PUNCTUATOR,
    ['['] = CHAR_PUNCTUATOR,
    [']'] = CHAR_PUNCTUATOR,
    ['.'] = CHAR_PUNCTUATOR,
    ['<'] = CHAR_PUNCTUATOR,
    ['>'] = CHAR_PUNCTUATOR,
    ['&'] = CHAR_PUNCTUATOR,
    ['^'] = CHAR_PUNCTUATOR,
    ['"'] = CHAR_PUNCTUATOR,
    ['\''] = CHAR_PUNCTUATOR,
};

static char *keywords[TOKEN_KEYWORD_UNKNOWN] = {
    [TOKEN_KEYWORD_AUTO]        = "auto",
    [TOKEN_KEYWORD_BREAK]       = "break",
    [TOKEN_KEYWORD_CASE]        = "case",
    [TOKEN_KEYWORD_CHAR]        = "char",
    [TOKEN_KEYWORD_CONST]       = "const",
    [TOKEN_KEYWORD_CONTINUE]    = "continue",
    [TOKEN_KEYWORD_DEFAULT]     = "default",
    [TOKEN_KEYWORD_DO]          = "do",
    [TOKEN_KEYWORD_DOUBLE]      = "double",
    [TOKEN_KEYWORD_ELSE]        = "else",
    [TOKEN_KEYWORD_ENUM]        = "enum",
    [TOKEN_KEYWORD_EXTERN]      = "extern",
    [TOKEN_KEYWORD_FLOAT]       = "float",
    [TOKEN_KEYWORD_FOR]         = "for",
    [TOKEN_KEYWORD_GOTO]        = "goto",
    [TOKEN_KEYWORD_IF]          = "if",
    [TOKEN_KEYWORD_INLINE]      = "inline",
    [TOKEN_KEYWORD_INT]         = "int",
    [TOKEN_KEYWORD_LONG]        = "long",
    [TOKEN_KEYWORD_REGISTER]    = "register",
    [TOKEN_KEYWORD_RESTRICT]    = "restrict",
    [TOKEN_KEYWORD_RETURN]      = "return",
    [TOKEN_KEYWORD_SHORT]       = "short",
    [TOKEN_KEYWORD_SIGNED]      = "signed",
    [TOKEN_KEYWORD_SIZEOF]      = "sizeof",
    [TOKEN_KEYWORD_STATIC]      = "static",
    [TOKEN_KEYWORD_STRUCT]      = "struct",
    [TOKEN_KEYWORD_SWITCH]      = "switch",
    [TOKEN_KEYWORD_TYPEDEF]     = "typedef",
    [TOKEN_KEYWORD_UNION]       = "union",
    [TOKEN_KEYWORD_UNSIGNED]    = "unsigned",
    [TOKEN_KEYWORD_VOID]        = "void",
    [TOKEN_KEYWORD_VOLATILE]    = "volatile",
    [TOKEN_KEYWORD_WHILE]       = "while",
};

struct token_t lex_token(char *text, uint32_t *offset, uint32_t *line, uint32_t *column)
{
    // uint32_t i;
    // uint32_t j;

    struct token_t token;
    static char token_text[8192];
    uint32_t token_text_index = 0;
    uint32_t token_type = TOKEN_EOF;
    uint32_t token_name = TOKEN_UNKNOWN;
    union constant_t constant;
    uint32_t reserved_token_type;
    uint32_t base;
    uint32_t has_point = 0;

    uint32_t text_offset = *offset;

    uint32_t cur_line = *line;
    uint32_t cur_column = *column;

    _try_again:
    switch(char_map[(uint32_t)text[text_offset]])
    {
        case CHAR_NUMBER:
            token_type = TOKEN_CONSTANT;
            token_name = TOKEN_CONSTANT_INTEGER;

            while(char_map[(uint32_t)text[text_offset]] & (CHAR_NUMBER | CHAR_LETTER))
            {
                token_text[token_text_index] = text[text_offset];
                if(text[text_offset] == '.')
                {
                    has_point++;
                }
                text_offset++;
                token_text_index++;
            }

            token_text[token_text_index] = '\0';
            token_text_index = 0;
            if(token_text[token_text_index] == '0' && !has_point)
            {
                /* hex or octal constant */
                token_text_index++;

                if(token_text[token_text_index] == 'x' ||
                   token_text[token_text_index] == 'X')
                {
                    base = 16;
                }
                else if(token_text[token_text_index])
                {
                    base = 8;
                }

                constant.int_constant = strtol(token_text, NULL, base);
            }
            else
            {
                if(!has_point)
                {
                    /* integer constant */
                    constant.int_constant = strtol(token_text, NULL, 10);
                }
                else
                {
                    /* maybe a float constant */
                }
            }

        break;

        case CHAR_LETTER:
        {
            token_type = TOKEN_IDENTIFIER;
            token_name = TOKEN_UNKNOWN;
            uint32_t start_offset = text_offset;

            while(char_map[(uint32_t)text[text_offset]] & (CHAR_LETTER | CHAR_NUMBER))
            {
                token_text[token_text_index] = text[text_offset];
                text_offset++;
                token_text_index++;
            }

            token_text[token_text_index] = '\0';

            /* check to see whether this is a reserved keyword... */
            for(uint32_t keyword_index = TOKEN_KEYWORD_FIRST; keyword_index < TOKEN_KEYWORD_LAST; keyword_index++)
            {
                if(!strcmp(keywords[keyword_index], token_text))
                {
                    token_type = TOKEN_KEYWORD;
                    token_name = keyword_index;
                    /* yup... */
                    break;
                }
            }

            if(token_type == TOKEN_IDENTIFIER)
            {
                // constant.string_constant = token_text;
                constant.string_constant = text + start_offset;
                constant.string_length = token_text_index;
            }
        }
        break;

        case CHAR_PUNCTUATOR:
            token_type = TOKEN_PUNCTUATOR;

            switch(text[text_offset])
            {
                case '+':
                    text_offset++;

                    if(text[text_offset] == '+')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_INCREMENT;
                    }
                    else if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_PLUS_ASSIGN;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_PLUS;
                    }

                break;

                case '-':
                    text_offset++;

                    if(text[text_offset] == '-')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_DECREMENT;
                    }
                    else if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_MINUS_ASSIGN;
                    }
                    else if(text[text_offset] == '>')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_ARROW;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_MINUS;
                    }
                break;

                case '*':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_MUL_ASSIGN;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_ASTERISC;
                    }

                break;

                case '/':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_DIV_ASSIGN;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_SLASH;
                    }
                break;

                case '%':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_MOD_ASSIGN;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_PERCENT;
                    }
                break;

                case '=':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_EQUAL;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_ASSIGN;
                    }
                break;

                case '<':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_LESS_THAN_OR_EQUAL;
                    }
                    else if(text[text_offset] == '<')
                    {
                        text_offset++;

                        if(text[text_offset] == '=')
                        {
                            text_offset++;
                            token_name = TOKEN_PUNCTUATOR_LEFT_SHIFT_ASSIGN;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_LEFT_SHIFT;
                        }
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_LESS_THAN;
                    }
                break;

                case '>':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_GREATER_THAN_OR_EQUAL;
                    }
                    else if(text[text_offset] == '>')
                    {
                        text_offset++;

                        if(text[text_offset] == '=')
                        {
                            text_offset++;
                            token_name = TOKEN_PUNCTUATOR_RIGHT_SHIFT_ASSIGN;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_RIGHT_SHIFT;
                        }
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_GREATER_THAN;
                    }
                break;

                case '(':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_OPARENTHESIS;
                break;

                case ')':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_CPARENTHESIS;
                break;

                case '{':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_OBRACE;
                break;

                case '}':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_CBRACE;
                break;

                case '[':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_OBRACKET;
                break;

                case ']':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_CBRACKET;
                break;

                case ',':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_COMMA;
                break;

                case '.':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_DOT;
                break;

                case ';':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_SEMICOLON;
                break;

                case ':':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_COLON;
                break;

                case '!':
                    text_offset++;

                    if(text[text_offset] == '=')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_NOT_EQUAL;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_EXCLAMATION;
                    }
                break;

                case '~':
                    text_offset++;
                    token_name = TOKEN_PUNCTUATOR_TILDE;
                break;

                case '&':
                    text_offset++;

                    if(text[text_offset] == '&')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_LOG_AND;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_AMPERSAND;
                    }

                break;

                case '|':
                    text_offset++;

                    if(text[text_offset] == '|')
                    {
                        text_offset++;
                        token_name = TOKEN_PUNCTUATOR_LOG_OR;
                    }
                    else
                    {
                        token_name = TOKEN_PUNCTUATOR_BW_OR;
                    }
                break;

                case '\'':

                break;

                case '"':
                    token_type = TOKEN_STRING_LITERAL;
                    text_offset++;
                    while(text[text_offset] != '"' && text[text_offset] != '\0')
                    {
                        token_text[token_text_index] = text[text_offset];
                        token_text_index++;
                        text_offset++;
                    }
                    text_offset++;
                    token_text[token_text_index] = '\0';
                    constant.string_constant = token_text;
                break;
            }
        break;

        case CHAR_UNKNOWN:
            text_offset++;
            token_type = TOKEN_UNKNOWN;
            token_name = TOKEN_UNKNOWN;
        break;

        case CHAR_SPACE:
            if(text[text_offset])
            {
                while(char_map[(uint32_t)text[text_offset]] == CHAR_SPACE)
                {
                    switch(text[text_offset])
                    {
                        case ' ':
                            cur_column++;
                        break;

                        case '\n':
                            cur_line++;
                            cur_column = 0;
                        break;
                    }
                    
                    text_offset++;
                }
                goto _try_again;
            }
            else
            {
                token_type = TOKEN_EOF;
            }
        break;
    }

    token.constant = constant;
    token.type = token_type;
    token.name = token_name;
    token.column = cur_column;
    token.line = cur_line;
    
    *column = cur_column + text_offset - *offset;
    *line = cur_line;
    *offset = text_offset;

    return token;
}

struct token_t *lex_tokens(char *text)
{
    uint32_t i = 0;

    struct token_t *tokens = NULL;
    struct token_t *last_token = NULL;
    struct token_t *new_token = NULL;
    struct token_t token;

    uint32_t column = 0;
    uint32_t line = 0;

    while(text[i])
    {
        token = lex_token(text, &i, &column, &line);
        if(token.type == TOKEN_UNKNOWN)
        {
            continue;
        }
        new_token = calloc(sizeof(struct token_t), 1);
        memcpy(new_token, &token, sizeof(struct token_t));
        if(new_token->type == TOKEN_IDENTIFIER ||
           new_token->type == TOKEN_STRING_LITERAL)
        {
            new_token->constant.string_constant = strdup(token.constant.string_constant);
        }

        if(!tokens)
        {
            tokens = new_token;
        }
        else
        {
            last_token->next = new_token;
        }

        last_token = new_token;
    }

    new_token = calloc(sizeof(struct token_t), 1);
    new_token->type = TOKEN_EOF;
    new_token->name = TOKEN_EOF;

    if(!tokens)
    {
        tokens = new_token;
    }
    else
    {
        last_token->next = new_token;
    }

    last_token = new_token;

    return tokens;
}

void free_tokens(struct token_t *tokens)
{
    struct token_t *next_token;

    while(tokens)
    {
        if(tokens->type == TOKEN_STRING_LITERAL ||
            tokens->type == TOKEN_IDENTIFIER);
        {
            free(tokens->constant.string_constant);
        }

        next_token = tokens->next;
        free(tokens);
        tokens = next_token;
    }
}

void translate_token(struct token_t *token)
{

    switch(token->type)
    {
        case TOKEN_EOF:
            printf("TOKEN_EOF\n");
        break;

        case TOKEN_CONSTANT:
//            printf("TOKEN_CONSTANT: %s\n", token->text);
        break;

        case TOKEN_STRING_LITERAL:
            printf("TOKEN_STRING_LITERAL: %s\n", token->constant.string_constant);
        break;

        case TOKEN_IDENTIFIER:
            printf("TOKEN_IDENTIFIER: %s\n", token->constant.string_constant);
        break;

        case TOKEN_KEYWORD:
            switch(token->name)
            {
                case TOKEN_KEYWORD_AUTO:
                    printf("TOKEN_KEYWORD_AUTO\n");
                break;

                case TOKEN_KEYWORD_BREAK:
                    printf("TOKEN_KEYWORD_BREAK\n");
                break;

                case TOKEN_KEYWORD_CASE:
                    printf("TOKEN_KEYWORD_CASE\n");
                break;

                case TOKEN_KEYWORD_CHAR:
                    printf("TOKEN_KEYWORD_CHAR\n");
                break;

                case TOKEN_KEYWORD_CONST:
                    printf("TOKEN_KEYWORD_CONST\n");
                break;

                case TOKEN_KEYWORD_CONTINUE:
                    printf("TOKEN_KEYWORD_CONTINUE\n");
                break;

                case TOKEN_KEYWORD_DEFAULT:
                    printf("TOKEN_KEYWORD_DEFAULT\n");
                break;

                case TOKEN_KEYWORD_DO:
                    printf("TOKEN_KEYWORD_DO\n");
                break;

                case TOKEN_KEYWORD_DOUBLE:
                    printf("TOKEN_KEYWORD_DOUBLE\n");
                break;

                case TOKEN_KEYWORD_ELSE:
                    printf("TOKEN_KEYWORD_ELSE\n");
                break;

                case TOKEN_KEYWORD_ENUM:
                    printf("TOKEN_KEYWORD_ENUM\n");
                break;

                case TOKEN_KEYWORD_EXTERN:
                    printf("TOKEN_KEYWORD_EXTERN\n");
                break;

                case TOKEN_KEYWORD_FLOAT:
                    printf("TOKEN_KEYWORD_FLOAT\n");
                break;

                case TOKEN_KEYWORD_FOR:
                    printf("TOKEN_KEYWORD_FOR\n");
                break;

                case TOKEN_KEYWORD_GOTO:
                    printf("TOKEN_KEYWORD_GOTO\n");
                break;

                case TOKEN_KEYWORD_IF:
                    printf("TOKEN_KEYWORD_IF\n");
                break;

                case TOKEN_KEYWORD_INLINE:
                    printf("TOKEN_KEYWORD_INLINE\n");
                break;

                case TOKEN_KEYWORD_INT:
                    printf("TOKEN_KEYWORD_INT\n");
                break;

                case TOKEN_KEYWORD_LONG:
                    printf("TOKEN_KEYWORD_LONG\n");
                break;

                case TOKEN_KEYWORD_REGISTER:
                    printf("TOKEN_KEYWORD_REGISTER\n");
                break;

                case TOKEN_KEYWORD_RESTRICT:
                    printf("TOKEN_KEYWORD_RESTRICT\n");
                break;

                case TOKEN_KEYWORD_RETURN:
                    printf("TOKEN_KEYWORD_RETURN\n");
                break;

                case TOKEN_KEYWORD_SHORT:
                    printf("TOKEN_KEYWORD_SHORT\n");
                break;

                case TOKEN_KEYWORD_SIGNED:
                    printf("TOKEN_KEYWORD_SIGNED\n");
                break;

                case TOKEN_KEYWORD_SIZEOF:
                    printf("TOKEN_KEYWORD_SIZEOF\n");
                break;

                case TOKEN_KEYWORD_STATIC:
                    printf("TOKEN_KEYWORD_STATIC\n");
                break;

                case TOKEN_KEYWORD_STRUCT:
                    printf("TOKEN_KEYWORD_STRUCT\n");
                break;

                case TOKEN_KEYWORD_SWITCH:
                    printf("TOKEN_KEYWORD_SWITCH\n");
                break;

                case TOKEN_KEYWORD_TYPEDEF:
                    printf("TOKEN_KEYWORD_TYPEDEF\n");
                break;

                case TOKEN_KEYWORD_UNION:
                    printf("TOKEN_KEYWORD_UNION\n");
                break;

                case TOKEN_KEYWORD_UNSIGNED:
                    printf("TOKEN_KEYWORD_UNSIGNED\n");
                break;

                case TOKEN_KEYWORD_VOID:
                    printf("TOKEN_KEYWORD_VOID\n");
                break;

                case TOKEN_KEYWORD_VOLATILE:
                    printf("TOKEN_KEYWORD_VOLATILE\n");
                break;

                case TOKEN_KEYWORD_WHILE:
                    printf("TOKEN_KEYWORD_WHILE\n");
                break;

                default:
                    printf("TOKEN_KEYWORD_UNKNOWN\n");
                break;

            }
        break;

        case TOKEN_PUNCTUATOR:
            switch(token->name)
            {
                case TOKEN_PUNCTUATOR_AMPERSAND:
                    printf("TOKEN_PUNCTUATOR_AMPERSAND\n");
                break;

                case TOKEN_PUNCTUATOR_ARROW:
                    printf("TOKEN_PUNCTUATOR_ARROW\n");
                break;

                case TOKEN_PUNCTUATOR_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_ASTERISC:
                    printf("TOKEN_PUNCTUATOR_ASTERISC\n");
                break;

                case TOKEN_PUNCTUATOR_BW_AND_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_BW_AND_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_BW_OR:
                    printf("TOKEN_PUNCTUATOR_BW_OR\n");
                break;

                case TOKEN_PUNCTUATOR_BW_OR_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_BW_OR_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_CBRACE:
                    printf("TOKEN_PUNCTUATOR_CBRACE\n");
                break;

                case TOKEN_PUNCTUATOR_CBRACKET:
                    printf("TOKEN_PUNCTUATOR_CBRACKET\n");
                break;

                case TOKEN_PUNCTUATOR_COLON:
                    printf("TOKEN_PUNCTUATOR_COLON\n");
                break;

                case TOKEN_PUNCTUATOR_COMMA:
                    printf("TOKEN_PUNCTUATOR_COMMA\n");
                break;

                case TOKEN_PUNCTUATOR_CPARENTHESIS:
                    printf("TOKEN_PUNCTUATOR_CPARENTHESIS\n");
                break;

                case TOKEN_PUNCTUATOR_DECREMENT:
                    printf("TOKEN_PUNCTUATOR_DECREMENT\n");
                break;

                case TOKEN_PUNCTUATOR_DIV_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_DIV_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_DOT:
                    printf("TOKEN_PUNCTUATOR_DOT\n");
                break;

                case TOKEN_PUNCTUATOR_EQUAL:
                    printf("TOKEN_PUNCTUATOR_EQUAL\n");
                break;

                case TOKEN_PUNCTUATOR_EXCLAMATION:
                    printf("TOKEN_PUNCTUATOR_EXCLAMATION\n");
                break;

                case TOKEN_PUNCTUATOR_GREATER_THAN:
                    printf("TOKEN_PUNCTUATOR_GREATER_THAN\n");
                break;

                case TOKEN_PUNCTUATOR_GREATER_THAN_OR_EQUAL:
                    printf("TOKEN_PUNCTUATOR_GREATER_THAN_OR_EQUAL\n");
                break;

                case TOKEN_PUNCTUATOR_INCREMENT:
                    printf("TOKEN_PUNCTUATOR_INCREMENT\n");
                break;

                case TOKEN_PUNCTUATOR_LEFT_SHIFT:
                    printf("TOKEN_PUNCTUATOR_LEFT_SHIFT\n");
                break;

                case TOKEN_PUNCTUATOR_LEFT_SHIFT_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_LEFT_SHIFT_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_LESS_THAN:
                    printf("TOKEN_PUNCTUATOR_LASS_THAN\n");
                break;

                case TOKEN_PUNCTUATOR_LESS_THAN_OR_EQUAL:
                    printf("TOKEN_PUNCTUATOR_LASS_THAN_OR_EQUAL\n");
                break;

                case TOKEN_PUNCTUATOR_LOG_AND:
                    printf("TOKEN_PUNCTUATOR_LOG_AND\n");
                break;

                case TOKEN_PUNCTUATOR_LOG_OR:
                    printf("TOKEN_PUNCTUATOR_LOG_OR\n");
                break;

                case TOKEN_PUNCTUATOR_MINUS:
                    printf("TOKEN_PUNCTUATOR_MINUS\n");
                break;

                case TOKEN_PUNCTUATOR_MINUS_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_MINUS_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_MOD_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_MOD_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_MUL_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_MUL_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_NOT_EQUAL:
                    printf("TOKEN_PUNCTUATOR_NOT_EQUAL\n");
                break;

                case TOKEN_PUNCTUATOR_OBRACE:
                    printf("TOKEN_PUNCTUATOR_OBRACE\n");
                break;

                case TOKEN_PUNCTUATOR_OBRACKET:
                    printf("TOKEN_PUNCTUATOR_OBRACKET\n");
                break;

                case TOKEN_PUNCTUATOR_OPARENTHESIS:
                    printf("TOKEN_PUNCTUATOR_OPARENTHESIS\n");
                break;

                case TOKEN_PUNCTUATOR_PERCENT:
                    printf("TOKEN_PUNCTUATOR_PERCENT\n");
                break;

                case TOKEN_PUNCTUATOR_PLUS:
                    printf("TOKEN_PUNCTUATOR_PLUS\n");
                break;

                case TOKEN_PUNCTUATOR_PLUS_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_PLUS_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_RIGHT_SHIFT:
                    printf("TOKEN_PUNCTUATOR_RIGHT_SHIFT\n");
                break;

                case TOKEN_PUNCTUATOR_RIGHT_SHIFT_ASSIGN:
                    printf("TOKEN_PUNCTUATOR_RIGHT_SHIFT_ASSIGN\n");
                break;

                case TOKEN_PUNCTUATOR_SEMICOLON:
                    printf("TOKEN_PUNCTUATOR_SEMICOLON\n");
                break;

                case TOKEN_PUNCTUATOR_SLASH:
                    printf("TOKEN_PUNCTUATOR_SLASH\n");
                break;

                case TOKEN_PUNCTUATOR_TILDE:
                    printf("TOKEN_PUNCTUATOR_TILDE\n");
                break;

                case TOKEN_PUNCTUATOR_XOR:
                    printf("TOKEN_PUNCTUATOR_XOR\n");
                break;

                default:
                    printf("TOKEN_PUNCTUATOR_UNKNOWN\n");
                break;
            }

        break;

        default:
            printf("TOKEN_UNKNOWN\n");
        break;
    }

}











