#ifndef OBJ_H
#define OBJ_H

#include "common.h"


struct object_t *create_object(struct parser_t *parser, struct declarator_t *declarator);

struct object_t *get_object(struct parser_t *parser, char *identifier);

int is_object_in_scope(struct parser_t *parser, struct object_t *object);





#endif // OBJ_H
