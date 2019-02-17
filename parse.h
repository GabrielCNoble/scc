#ifndef PARSE_H
#define PARSE_H


#include "tok.h"
#include "type.h"


enum PARSER_STATE
{
    PARSER_STATE_NONE = 0,
    PARSER_STATE_DECL = 1,
    PARSER_STATE_PARAM_LIST = 1 << 1,
};


struct parser_t
{
    struct token_t *tokens;
    struct token_t *current_token;
    struct token_t *prev_token;


    /*==========================*/


    int param_list_level;
};

int is_type_specifier(struct token_t *token);

int is_type_qualifier(struct token_t *token);

int is_storage_class_specifier(struct token_t *token);

int type_from_token(struct token_t *token);

char *type_string(struct base_type_t *type);

void advance_token(struct parser_t *parser);

void translate_type(struct base_type_t *type, int verbose);

void parse_tokens(struct token_t *tokens);

struct base_type_t *parse_declaration(struct parser_t *parser);

struct base_type_t *parse_declarator(struct parser_t *parser);









#endif // PARSE_H
