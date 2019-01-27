#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static int tokenizer_initialized = 0;

static char char_map[255] = {CHAR_UNKNOWN};

static char *reserved[RESERVED_TOKEN_UNKNOWN] = {NULL};

void token_init()
{
    int i;

    for(i = '0'; i < '9'; i++)
    {
        char_map[i] = CHAR_NUMBER;
    }

    for(i = 'a'; i < 'z'; i++)
    {
        char_map[i] = CHAR_LETTER;
    }

    for(i = 'A'; i < 'Z'; i++)
    {
        char_map[i] = CHAR_LETTER;
    }

    char_map['_'] = CHAR_LETTER;

    char_map['+'] = CHAR_SEPARATOR;
    char_map['-'] = CHAR_SEPARATOR;
    char_map['/'] = CHAR_SEPARATOR;
    char_map['*'] = CHAR_SEPARATOR;
    char_map[';'] = CHAR_SEPARATOR;
    char_map['('] = CHAR_SEPARATOR;
    char_map[')'] = CHAR_SEPARATOR;
    char_map[' '] = CHAR_SPACE;
    char_map[','] = CHAR_SEPARATOR;
    char_map['='] = CHAR_SEPARATOR;
    char_map['!'] = CHAR_SEPARATOR;



    reserved[RESERVED_TOKEN_FOR] = "for";
    reserved[RESERVED_TOKEN_IF]  = "if";
    reserved[RESERVED_TOKEN_ELSE] = "else";
    reserved[RESERVED_TOKEN_WHILE] = "while";
    reserved[RESERVED_TOKEN_DO] = "do";
    reserved[RESERVED_TOKEN_VOID] = "void";
    reserved[RESERVED_TOKEN_INT]  = "int";
    reserved[RESERVED_TOKEN_SHORT] = "short";
    reserved[RESERVED_TOKEN_CHAR] = "char";


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
                token_type = TOKEN_NUMBER;

                while(char_map[(int)text[i]] & CHAR_NUMBER)
                {
                    token_text[token_text_index] = text[i];
                    i++;
                    token_text_index++;
                }

                token_text[token_text_index] = '\0';

            break;

            case CHAR_LETTER:
                token_type = TOKEN_ID;

                while(char_map[(int)text[i]] & (CHAR_LETTER | CHAR_NUMBER))
                {
                    token_text[token_text_index] = text[i];
                    i++;
                    token_text_index++;
                }

                token_text[token_text_index] = '\0';

                j = RESERVED_TOKEN_FOR;
                /* check to see whether this is a reserved keyword... */
                for(j = RESERVED_TOKEN_FOR; j < RESERVED_TOKEN_UNKNOWN; j++)
                {
                    if(!strcmp(reserved[j], token_text))
                    {
                        token_type = TOKEN_RESERVED;
                        reserved_token_type = j;
                        /* yup... */
                        break;
                    }
                }

            break;

            case CHAR_SEPARATOR:

                switch(text[i])
                {
                    case '+':
                        i++;

                        if(text[i] == '+')
                        {
                            i++;
                            token_type = TOKEN_INCREMENT;
                        }
                        else if(text[i] == '=')
                        {
                            i++;
                            token_type = TOKEN_PLUS_ASSIGN;
                        }
                        else
                        {
                            token_type = TOKEN_PLUS;
                        }

                    break;

                    case '-':
                        i++;

                        if(text[i] == '-')
                        {
                            i++;
                            token_type = TOKEN_DECREMENT;
                        }
                        else if(text[i] == '=')
                        {
                            i++;
                            token_type = TOKEN_MINUS_ASSIGN;
                        }
                        else
                        {
                            token_type = TOKEN_MINUS;
                        }
                    break;

                    case '*':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_type = TOKEN_MUL_ASSIGN;
                        }
                        else
                        {
                            token_type = TOKEN_ASTERISC;
                        }

                    break;

                    case '/':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_type = TOKEN_DIV_ASSIGN;
                        }
                        else
                        {
                            token_type = TOKEN_DIV;
                        }
                    break;

                    case '=':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_type = TOKEN_EQUALS;
                        }
                        else
                        {
                            token_type = TOKEN_ASSIGN;
                        }
                    break;

                    case '(':
                        i++;
                        token_type = TOKEN_OPARENTHESIS;
                    break;

                    case ')':
                        i++;
                        token_type = TOKEN_CPARENTHESIS;
                    break;

                    case ',':
                        i++;
                        token_type = TOKEN_COMMA;
                    break;

                    case ';':
                        i++;
                        token_type = TOKEN_SEMICOLON;
                    break;

                    case '!':
                        i++;

                        if(text[i] == '=')
                        {
                            i++;
                            token_type = TOKEN_NOT_EQUALS;
                        }
                        else
                        {
                            token_type = TOKEN_LOGICAL_NOT;
                        }
                    break;
                }
            break;

            case CHAR_UNKNOWN:
                i++;
                token_type = TOKEN_UNKNOWN;
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
        new_token->token = token_type;
        new_token->reserved_token = reserved_token_type;

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

void translate_tokens(struct token_t *token)
{
    while(token)
    {
        switch(token->token)
        {
            case TOKEN_NUMBER:
                printf("TOKEN_NUMBER: %s\n", token->text);
            break;

            case TOKEN_ID:
                printf("TOKEN_ID: %s\n", token->text);
            break;

            case TOKEN_PLUS:
                printf("TOKEN_PLUS\n");
            break;

            case TOKEN_MINUS:
                printf("TOKEN_MINUS\n");
            break;

            case TOKEN_DIV:
                printf("TOKEN_DIV\n");
            break;

            case TOKEN_ASTERISC:
                printf("TOKEN_ASTERISC\n");
            break;

            case TOKEN_OPARENTHESIS:
                printf("TOKEN_OPARENTHESIS\n");
            break;

            case TOKEN_CPARENTHESIS:
                printf("TOKEN_CPARENTHESIS\n");
            break;

            case TOKEN_UNKNOWN:
                printf("TOKEN_UNKNOWN\n");
            break;
        }

        token = token->next;
    }
}











