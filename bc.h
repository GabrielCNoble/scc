#ifndef BC_H
#define BC_H




enum BYTECODE_INSTRUCTION
{
    BYTECODE_INSTRUCTION_HLT = 0,

    BYTECODE_INSTRUCTION_MOV,
    BYTECODE_INSTRUCTION_ADD,
    BYTECODE_INSTRUCTION_SUB,
    BYTECODE_INSTRUCTION_MUL,
    BYTECODE_INSTRUCTION_DIV,
    BYTECODE_INSTRUCTION_INC,
    BYTECODE_INSTRUCTION_DEC,
    BYTECODE_INSTRUCTION_LSH,
    BYTECODE_INSTRUCTION_RSH,
    BYTECODE_INSTRUCTION_CMP,

    BYTECODE_INSTRUCTION_PUSH,
    BYTECODE_INSTRUCTION_POP,

    BYTECODE_INSTRUCTION_CALL,
    BYTECODE_INSTRUCTION_RET,
    BYTECODE_INSTRUCTION_JMP,
    BYTECODE_INSTRUCTION_JZ,
    BYTECODE_INSTRUCTION_JNZ,
    BYTECODE_INSTRUCTION_JG,
    BYTECODE_INSTRUCTION_JNG,
    BYTECODE_INSTRUCTION_JL,
    BYTECODE_INSTRUCTION_JNL,
};


enum BYTECODE_OPERANDS
{
    BYTECODE_OPERAND_MEMORY = 0,
    BYTECODE_OPERAND_MEMORY_REAL,
    BYTECODE_OPERAND_REGSP,
    BYTECODE_OPERAND_REGBP,
    BYTECODE_OPERAND_REGA,
    BYTECODE_OPERAND_REGB,
    BYTECODE_OPERAND_REGC,
    BYTECODE_OPERAND_REGD,
};


#define DST_OPERAND_SHIFT 0x4


struct bytecode_t
{
    unsigned char instruction;
    unsigned char operands;
    unsigned short align;
};


struct bytecode_buffer_t
{
    unsigned char *bytecode;
    unsigned int buffer_size;
    unsigned int buffer_cursor;
};



void emit_bytecode(struct bytecode_buffer_t *buffer, unsigned char instruction, unsigned char dst, unsigned char src, unsigned int param);




#endif // BC_H








