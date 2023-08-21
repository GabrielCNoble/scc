#ifndef TOK_H
#define TOK_H

#include "common.h"

struct token_t lex_token(char *text, uint32_t *offset);

struct token_t *lex_tokens(char *text);

void free_tokens(struct token_t *tokens);

void translate_token(struct token_t *token);




#endif // TOK_H








