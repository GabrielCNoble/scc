#ifndef PARSE_H
#define PARSE_H


#include "tok.h"
#include "type.h"
#include "scope.h"
#include "obj.h"
#include "bc.h"


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

    int declaration_depth;


    int param_list_level;
    int aggregate_level;

    struct aggretage_type_t *aggregate_types;
    struct base_type_t *typedef_types;

    int scope_stack_top;
    struct scope_t **scope_stack;
    struct scope_t *current_scope;

    struct bytecode_buffer_t global_bytecode;

    unsigned int reg_index;

    //struct object_t *objects;
};


enum DECL_TOKENS
{
    DECL_TOKEN_QUALIFIER = 0,
    DECL_TOKEN_SPECIFIER,
    DECL_TOKEN_STORAGE_CLASS,
    DECL_TOKEN_DECLARATOR,
};


int is_type_specifier(struct token_t *token);

int is_type_qualifier(struct token_t *token);

int is_storage_class_specifier(struct token_t *token);

int is_typedef_name(struct token_t *token);

int type_from_token(struct token_t *token);

char *type_string(struct base_type_t *type);

void advance_token(struct parser_t *parser);

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

struct scope_t *push_scope(struct parser_t *parser);

struct scope_t *pop_scope(struct parser_t *parser);

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

struct object_t *create_object(struct parser_t *parser, struct base_type_t *type);

struct object_t *get_object(struct parser_t *parser, char *id);

int is_object_in_scope(struct parser_t *parser, struct object_t *object);

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

void stash_aggregate_type(struct parser_t *parser, struct aggretage_type_t *type);

struct base_type_t *stash_typedef_type(struct parser_t *parser, struct base_type_t *type);

struct base_type_t *get_aggregate_type(struct parser_t *parser, char *name);

struct base_type_t *copy_type(struct base_type_t *type);

int validate_type(struct base_type_t *type);

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

void parse_tokens(struct token_t *tokens);

struct base_type_t *parse_declaration(struct parser_t *parser, int is_in_arg_list);

struct base_type_t *parse_declarator(struct parser_t *parser);

struct base_type_t *parse_aggregate_definition(struct parser_t *parser);

void parse_compound_statement(struct parser_t *parser, struct bytecode_buffer_t *bytecode_buffer);




void translate_type(struct reference_type_t *type, int single_reference);




#endif // PARSE_H
