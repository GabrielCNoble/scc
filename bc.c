#include "bc.h"



void emit_bytecode(struct bytecode_buffer_t *buffer, unsigned char instruction, unsigned char dst, unsigned char src, unsigned int param)
{
    struct bytecode_t bytecode;
    unsigned char *bytecodes;

    int instruction_size;

    bytecode.instruction = instruction;
    bytecode.operands = (dst << DST_OPERAND_SHIFT) | src;

    instruction_size = sizeof(struct bytecode_t) + (src == BYTECODE_OPERAND_MEMORY || dst == BYTECODE_OPERAND_MEMORY) ? sizeof(param) : 0;

    if(buffer->buffer_cursor + instruction_size > buffer->buffer_size)
    {
        buffer->buffer_size += 64;
        bytecodes = calloc(buffer->buffer_size, 1);
        memcpy(bytecodes, buffer->bytecode, buffer->buffer_cursor);
        free(buffer->bytecode);

        buffer->bytecode = bytecodes;
    }

    memcpy(buffer->bytecode + buffer->buffer_cursor, &bytecode, sizeof(struct bytecode_t));
    buffer->buffer_cursor += sizeof(struct bytecode_t);

    if(src == BYTECODE_OPERAND_MEMORY || dst == BYTECODE_OPERAND_MEMORY)
    {
        memcpy(buffer->bytecode + buffer->buffer_cursor, &param, sizeof(param));
        buffer->buffer_cursor += sizeof(param);
    }
}





