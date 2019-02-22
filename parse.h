#ifndef PARSE_H
#define PARSE_H


#include "tok.h"
#include "type.h"
#include "scope.h"
#include "obj.h"


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
    int aggregate_level;

    struct aggretage_type_t *aggregate_types;
    struct base_type_t *typedef_types;

    int scope_stack_top;
    struct scope_t **scope_stack;
    struct scope_t *current_scope;

    struct object_t *objects;
};

int is_type_specifier(struct token_t *token);

int is_type_qualifier(struct token_t *token);

int is_storage_class_specifier(struct token_t *token);

int is_typedef_name(struct token_t *token);

int type_from_token(struct token_t *token);

char *type_string(struct base_type_t *type);

void advance_token(struct parser_t *parser);

struct scope_t *push_scope(struct parser_t *parser);

struct scope_t *pop_scope(struct parser_t *parser, int delete_scope);

struct object_t *create_object(struct parser_t *parser, struct base_type_t *type);

int is_object_in_scope(struct parser_t *parser, struct object_t *object);





void stash_aggregate_type(struct parser_t *parser, struct aggretage_type_t *type);

struct base_type_t *stash_typedef_type(struct parser_t *parser, struct base_type_t *type);

struct base_type_t *get_aggregate_type(struct parser_t *parser, char *name);



//void push_type(struct parser_t *parser, struct base_type_t *type);


void parse_tokens(struct token_t *tokens);

struct base_type_t *parse_declaration(struct parser_t *parser);

struct base_type_t *parse_declarator(struct parser_t *parser);

struct base_type_t *parse_aggregate_definition(struct parser_t *parser);




void translate_type(struct reference_type_t *type, int single_reference);




#endif // PARSE_H
