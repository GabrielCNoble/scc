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

    char_map['+'] = CHAR_PLUS;
    char_map['-'] = CHAR_MINUS;
    char_map['/'] = CHAR_BAR;
    char_map['*'] = CHAR_ASTERISC;
    char_map[';'] = CHAR_SEMICOLON;
    char_map['('] = CHAR_OPARENTHESIS;
    char_map[')'] = CHAR_CPARENTHESIS;
    char_map[' '] = CHAR_SPACE;
    char_map[','] = CHAR_COMMA;
    char_map['='] = CHAR_EQUALS;
    char_map['!'] = CHAR_EXCLAMATION;



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
        int char_type = char_map[(int)text[i]];

        switch(char_type)
        {
            case CHAR_NUMBER:
                token_type = TOKEN_NUMBER;
            break;

            case CHAR_LETTER:
                token_type = TOKEN_ID;
            break;

            case CHAR_PLUS:

                i++;

                switch(text[i])
                {
                    case '+':
                        i++;
                        token_type = TOKEN_INCREMENT;
                    break;

                    case '=':
                        i++;
                        token_type = TOKEN_PLUS_ASSIGN;
                    break;

                    default:
                        token_type = TOKEN_PLUS;
                    break;
                }

                goto _add_new_token;
            break;

            case CHAR_MINUS:

                i++;

                switch(text[i])
                {
                    case '-':
                        i++;
                        token_type = TOKEN_DECREMENT;
                    break;

                    case '=':
                        i++;
                        token_type = TOKEN_MINUS_ASSIGN;
                    break;

                    default:
                        token_type = TOKEN_MINUS;
                    break;
                }

                goto _add_new_token;
            break;

            case CHAR_BAR:

                i++;

                switch(text[i])
                {
                    case '=':
                        i++;
                        token_type = TOKEN_DIV_ASSIGN;
                    break;

                    default:
                        token_type = TOKEN_DIV;
                    break;
                }

                goto _add_new_token;
            break;

            case CHAR_ASTERISC:

                i++;

                switch(text[i])
                {
                    case '=':
                        i++;
                        token_type = TOKEN_MUL_ASSIGN;
                    break;

                    default:
                        token_type = TOKEN_MUL;
                    break;
                }

                goto _add_new_token;
            break;

            case CHAR_OPARENTHESIS:
                token_type = TOKEN_OPARENTHESIS;
                i++;
                goto _add_new_token;
            break;

            case CHAR_CPARENTHESIS:
                token_type = TOKEN_CPARENTHESIS;
                i++;
                goto _add_new_token;
            break;

            case CHAR_COMMA:
                token_type = TOKEN_COMMA;
                i++;
                goto _add_new_token;
            break;

            case CHAR_EQUALS:
                i++;

                switch(text[i])
                {
                    case '=':
                        i++;
                        token_type = TOKEN_EQUALS;
                    break;

                    default:
                        token_type = TOKEN_ASSIGN;
                    break;
                }

                goto _add_new_token;
            break;

            case CHAR_EXCLAMATION:
                i++;

                switch(text[i])
                {
                    case '=':
                        i++;
                        token_type = TOKEN_NOT_EQUALS;
                    break;

                    default:
                        token_type = TOKEN_LOGICAL_NOT;
                    break;
                }

                goto _add_new_token;
            break;

            case CHAR_SEMICOLON:
                i++;
                token_type = TOKEN_SEMICOLON;
                goto _add_new_token;
            break;



            case CHAR_UNKNOWN:
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

        token_text_index = 0;

        while(char_map[(int)text[i]] == char_type)
        {
            token_text[token_text_index] = text[i];

            i++;
            token_text_index++;
        }

        token_text[token_text_index] = '\0';


        if(token_type == TOKEN_ID)
        {
            j = RESERVED_TOKEN_FOR;

            while(reserved[j])
            {
                if(!strcmp(reserved[j], token_text))
                {
                    token_type = TOKEN_RESERVED;
                    reserved_token_type = j;
                    break;
                }

                j++;
            }
        }

        _add_new_token:

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

            case TOKEN_MUL:
                printf("TOKEN_MUL\n");
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











