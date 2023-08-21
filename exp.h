#ifndef EXP_H
#define EXP_H

#include "common.h"



struct base_exp_node_t *parse_exp(struct parser_t *parser);

struct base_exp_node_t *comma_exp(struct parser_t *parser);

struct base_exp_node_t *assignment_exp(struct parser_t *parser);

struct base_exp_node_t *conditional_exp(struct parser_t *parser);

struct base_exp_node_t *logical_or_exp(struct parser_t *parser);

struct base_exp_node_t *logical_and_exp(struct parser_t *parser);

struct base_exp_node_t *or_exp(struct parser_t *parser);

struct base_exp_node_t *xor_exp(struct parser_t *parser);

struct base_exp_node_t *and_exp(struct parser_t *parser);

struct base_exp_node_t *equality_exp(struct parser_t *parser);

struct base_exp_node_t *relational_exp(struct parser_t *parser);

struct base_exp_node_t *shift_exp(struct parser_t *parser);

struct base_exp_node_t *additive_exp(struct parser_t *parser);

struct base_exp_node_t *multiplicative_exp(struct parser_t *parser);

struct base_exp_node_t *cast_exp(struct parser_t *parser);

struct base_exp_node_t *unary_exp(struct parser_t *parser);

struct base_exp_node_t *postfix_exp(struct parser_t *parser);

struct base_exp_node_t *primary_exp(struct parser_t *parser);

void traverse_exp_tree(struct base_exp_node_t *exp);



#endif // EXP_H








