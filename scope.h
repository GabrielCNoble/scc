#ifndef SCOPE_H
#define SCOPE_H

#include "common.h"

struct scope_t *push_scope(struct parser_t *parser);

struct scope_t *pop_scope(struct parser_t *parser);

#endif // SCOPE_H
