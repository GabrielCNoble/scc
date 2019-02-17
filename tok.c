#include "tok.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static int tokenizer_initialized = 0;

static char char_map[255] = {CHAR_UNKNOWN};

static char *keywords[TOKEN_KEYWORD_UNION] = {NULL};
//static char *punctuator[TOKEN_PUNCTUATOR_UNKNOWN] = {NULL};

void token_init()
{
    int i;

    for(i = '0'; i <= '9'; i++)
    {
        char_map[i] = CHAR_NUMBER;
    }

    for(i = 'a'; i <= 'z'; i++)
    {
        char_map[i] = CHAR_LETTER;
    }

    for(i = 'A'; i <= 'Z'; i++)
    {
        char_map[i] = CHAR_LETTER;
    }

    char_map['_'] = CHAR_LETTER;

    char_map['+'] = CHAR_PUNCTUATOR;
    char_map['-'] = CHAR_PUNCTUATOR;
    char_map['/'] = CHAR_PUNCTUATOR;
    char_map['*'] = CHAR_PUNCTUATOR;
    char_map['%'] = CHAR_PUNCTUATOR;
    char_map[';'] = CHAR_PUNCTUATOR;
    char_map[':'] = CHAR_PUNCTUATOR;
    char_map['('] = CHAR_PUNCTUATOR;
    char_map[')'] = CHAR_PUNCTUATOR;
    char_map[' '] = CHAR_SPACE;
    char_map[','] = CHAR_PUNCTUATOR;
    char_map['='] = CHAR_PUNCTUATOR;
    char_map['!'] = CHAR_PUNCTUATOR;
    char_map['~'] = CHAR_PUNCTUATOR;
    char_map['{'] = CHAR_PUNCTUATOR;
    char_map['}'] = CHAR_PUNCTUATOR;
    char_map['['] = CHAR_PUNCTUATOR;
    char_map[']'] = CHAR_PUNCTUATOR;
    char_map['.'] = CHAR_PUNCTUATOR;
    char_map['<'] = CHAR_PUNCTUATOR;
    char_map['>'] = CHAR_PUNCTUATOR;
    char_map['&'] = CHAR_PUNCTUATOR;
    char_map['^'] = CHAR_PUNCTUATOR;

    keywords[TOKEN_KEYWORD_AUTO] = "auto";
    keywords[TOKEN_KEYWORD_BREAK] = "break";
    keywords[TOKEN_KEYWORD_CASE] = "case";
    keywords[TOKEN_KEYWORD_CHAR] = "char";
    keywords[TOKEN_KEYWORD_CONST] = "const";
    keywords[TOKEN_KEYWORD_CONTINUE] = "continue";
    keywords[TOKEN_KEYWORD_DEFAULT] = "default";
    keywords[TOKEN_KEYWORD_DO] = "do";
    keywords[TOKEN_KEYWORD_DOUBLE] = "double";
    keywords[TOKEN_KEYWORD_ELSE] = "else";
    keywords[TOKEN_KEYWORD_ENUM] = "enum";
    keywords[TOKEN_KEYWORD_EXTERN] = "extern";
    keywords[TOKEN_KEYWORD_FLOAT] = "float";
    keywords[TOKEN_KEYWORD_FOR] = "for";
    keywords[TOKEN_KEYWORD_GOTO] = "goto";
    keywords[TOKEN_KEYWORD_IF] = "if";
    keywords[TOKEN_KEYWORD_INLINE] = "inline";
    keywords[TOKEN_KEYWORD_INT] = "int";
    keywords[TOKEN_KEYWORD_LONG] = "long";
    keywords[TOKEN_KEYWORD_REGISTER] = "register";
    keywords[TOKEN_KEYWORD_RESTRICT] = "restrict";
    keywords[TOKEN_KEYWORD_RETURN] = "return";
    keywords[TOKEN_KEYWORD_SHORT] = "short";
    keywords[TOKEN_KEYWORD_SIGNED] = "signed";
    keywords[TOKEN_KEYWORD_SIZEOF] = "sizeof";
    keywords[TOKEN_KEYWORD_STATIC] = "static";
    keywords[TOKEN_KEYWORD_STRUCT] = "struct";
    keywords[TOKEN_KEYWORD_SWITCH] = "switch";
    keywords[TOKEN_KEYWORD_TYPEDEF] = "typedef";
    keywords[TOKEN_KEYWORD_UNION] = "union";
    keywords[TOKEN_KEYWORD_UNSIGNED] = "unsigned";
    keywords[TOKEN_KEYWORD_VOID] = "void";
    keywords[TOKEN_KEYWORD_VOLATILE] = "volatile";
    keywords[TOKEN_KEYWORD_WHILE] = "while";

    tokenizer_initialized = 1;
}

struct token_t *tokenize(char *text)
{
    int i;
    int j;

    struct token_t *tokens = NULL;
    struct token_t *last_token = NULL;
    struct token_t *new_token = NULL;

    char token_text[512];
    int token_text_index;
    int token_type;
    int token_name;
    int reserved_token_type;

    if(!tokenizer_initialized)
    {
        token_init();
    }

    i = 0;

    while(text[i])
    {
        //int char_type = char_map[(int)text[i]];

        token_text[0] = '\0';
        token_text_index = 0;
        switch(char_map[(int)text[i]])
        {
            case CHAR_NUMBER:
                token_type = TOKEN_CONSTANT;
                token_name = TOKEN_UNKNOWN;

                while(char_map[(int)text[i]] & CHAR_NUMBER)
                {
                    token_text[token_text_index] = text[i];
                    i++;
                    token_text_index++;
                }

                token_text[token_text_index] = '\0';

            break;

            case CHAR_LETTER:
                token_type = TOKEN_IDENTIFIER;
                token_name = TOKEN_UNKNOWN;

                while(char_map[(int)text[i]] & (CHAR_LETTER | CHAR_NUMBER))
                {
                    token_text[token_text_index] = text[i];
                    i++;
                    token_text_index++;
                }

                token_text[token_text_index] = '\0';

                /* check to see whether this is a reserved keyword... */
                for(j = TOKEN_KEYWORD_AUTO; j < TOKEN_KEYWORD_WHILE; j++)
                {
                    if(!strcmp(keywords[j], token_text))
                    {
                        token_type = TOKEN_KEYWORD;
                        token_name = j;
                        /* yup... */
                        break;
                    }
                }

            break;

            case CHAR_PUNCTUATOR:
                token_type = TOKEN_PUNCTUATOR;

                switch(text[i])
                {
                    case '+':
                        i++;

                        if(text[i] == '+')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_INCREMENT;
                        }
                        else if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_PLUS_ASSIGN;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_PLUS;
                        }

                    break;

                    case '-':
                        i++;

                        if(text[i] == '-')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_DECREMENT;
                        }
                        else if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_MINUS_ASSIGN;
                        }
                        else if(text[i] == '>')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_ARROW;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_MINUS;
                        }
                    break;

                    case '*':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_MUL_ASSIGN;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_ASTERISC;
                        }

                    break;

                    case '/':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_DIV_ASSIGN;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_SLASH;
                        }
                    break;

                    case '%':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_MOD_ASSIGN;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_PERCENT;
                        }
                    break;

                    case '=':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_EQUAL;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_ASSIGN;
                        }
                    break;

                    case '<':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_LESS_THAN_OR_EQUAL;
                        }
                        else if(text[i] == '<')
                        {
                            i++;

                            if(text[i] == '=')
                            {
                                i++;
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
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_GREATER_THAN_OR_EQUAL;
                        }
                        else if(text[i] == '>')
                        {
                            i++;

                            if(text[i] == '=')
                            {
                                i++;
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
                        i++;
                        token_name = TOKEN_PUNCTUATOR_OPARENTHESIS;
                    break;

                    case ')':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_CPARENTHESIS;
                    break;

                    case '{':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_OBRACE;
                    break;

                    case '}':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_CBRACE;
                    break;

                    case '[':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_OBRACKET;
                    break;

                    case ']':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_CBRACKET;
                    break;

                    case ',':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_COMMA;
                    break;

                    case ';':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_SEMICOLON;
                    break;

                    case ':':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_COLON;
                    break;

                    case '!':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_NOT_EQUAL;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_EXCLAMATION;
                        }
                    break;

                    case '~':
                        i++;
                        token_name = TOKEN_PUNCTUATOR_TILDE;
                    break;

                    case '&':
                        i++;

                        if(text[i] == '&')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_LOG_AND;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_AMPERSAND;
                        }

                    break;

                    case '|':
                        i++;

                        if(text[i] == '|')
                        {
                            i++;
                            token_name = TOKEN_PUNCTUATOR_LOG_OR;
                        }
                        else
                        {
                            token_name = TOKEN_PUNCTUATOR_BW_OR;
                        }
                    break;
                }
            break;

            case CHAR_UNKNOWN:
                i++;
                token_type = TOKEN_UNKNOWN;
                token_name = TOKEN_UNKNOWN;
            break;

            case CHAR_SPACE:

                while(char_map[(int)text[i]] == CHAR_SPACE)
                {
                    i++;
                }

                continue;
            break;
        }

        new_token = calloc(sizeof(struct token_t), 1);
        strcpy(new_token->text, token_text);
        new_token->token_type = token_type;
        new_token->token_name = token_name;

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
    new_token->token_type = TOKEN_EOF;
    new_token->token_name = TOKEN_EOF;

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
        if(tokens->text)
        {
            free(tokens->text);
        }

        next_token = tokens->next;
        free(tokens);
        tokens = next_token;
    }
}

void translate_token(struct token_t *token)
{

    switch(token->token_type)
    {
        case TOKEN_EOF:
            printf("TOKEN_EOF\n");
        break;

        case TOKEN_CONSTANT:
            printf("TOKEN_CONSTANT: %s\n", token->text);
        break;

        case TOKEN_IDENTIFIER:
            printf("TOKEN_IDENTIFIER: %s\n", token->text);
        break;

        case TOKEN_KEYWORD:
            switch(token->token_name)
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
            switch(token->token_name)
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











