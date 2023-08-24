#ifndef PARSE_H
#define PARSE_H


//#include <stdint.h>
#include "common.h"
//#include "type.h"
//#include "scope.h"
//#include "obj.h"
//#include "exp.h"
//#include "bc.h"




//enum PARSER_STATE
//{
//    PARSER_STATE_NONE = 0,
//    PARSER_STATE_DECL = 1,
//    PARSER_STATE_PARAM_LIST = 1 << 1,
//};



int is_type_specifier(struct token_t *token);

int is_type_qualifier(struct token_t *token);

int is_storage_class_specifier(struct token_t *token);

int is_declaration_specifier(struct token_t *token);

int is_typedef_name(struct token_t *token);

uint32_t type_specifier_from_token(struct token_t *token);

char *type_string(struct base_type_t *type);

void advance_token(struct parser_t *parser);


/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

struct type_t *stash_aggregate_type(struct parser_t *parser, struct type_t *type);

struct type_t *get_aggregate_type(struct parser_t *parser, char *name);

struct base_type_t *stash_typedef_type(struct parser_t *parser, struct base_type_t *type);

struct base_type_t *copy_type(struct base_type_t *type);

//int validate_type(struct base_type_t *type);

/*
==========================================================================================
==========================================================================================
==========================================================================================
*/

//void parse_tokens(struct token_t *tokens);

void error(uint32_t line, uint32_t column, const char *format, ...); 

void parse(char *text);

struct declarator_t *parse_declaration(struct parser_t *parser, uint32_t in_arg_list);

struct declarator_t *parse_declarator(struct parser_t *parser, struct declarator_t *declarator);

struct base_type_t *parse_initializer(struct parser_t *parser);

struct type_t *parse_aggregate_declaration(struct parser_t *parser);



void parse_statement(struct parser_t *parser);

void parse_labeled_statement(struct parser_t *parser); 

void parse_compound_statement(struct parser_t *parser);

void parse_selection_statement(struct parser_t *parser);

void parse_iteration_statement(struct parser_t *parser);

void parse_jump_statement(struct parser_t *parser);

void parse_expression_statement(struct parser_t *parser);



void translate_expression(struct base_exp_node_t *exp);

void translate_type(struct link_type_t *type, int single_reference);




#endif // PARSE_H
