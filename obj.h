#ifndef OBJ_H
#define OBJ_H

#include "common.h"


struct object_t *create_object(struct parser_t *parser, struct base_type_t *type);

struct object_t *get_object(struct parser_t *parser, char *id);

int is_object_in_scope(struct parser_t *parser, struct object_t *object);





#endif // OBJ_H
