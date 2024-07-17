#include <stdio.h>

#include "value.h"
#include "debug.h"

void disassembleChunk(Chunk* chunk, const char* name)
{
	printf("== %s == \n", name);

	for (int offset = 0; offset < chunk->count;) {
		offset = disassembleInstruction(chunk, offset);
	}
}

static int simpleInstruction(const char* name, int offset) 
{
	printf("%s\n", name);
	return offset + 1;
}

static int byteInstruction(const char* name, Chunk* chunk, int offset)
{
	uint8_t slot = chunk->code[offset + 1];
	printf("%-16s %4d \n", name, slot);
	return offset + 2;
}

static int jumpInstruction(const char* name, int sign, Chunk* chunk, int offset)
{
	uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
	jump |= chunk->code[offset + 2];
	printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
	return offset + 3;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset)
{
	uint8_t constant = chunk->code[offset + 1];
	printf("%-16s %4d '", name, constant);
	printValue(chunk->constants.values[constant]);
	printf("'\n");
	return offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset)
{
	printf("%04d ", offset);

	if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
	{
		printf("  |  ");
	}
	else
	{
		printf("%4d ", chunk->lines[offset]);
	}

	uint8_t instruction = chunk->code[offset];
	switch (instruction) 
	{
	case OP_CONSTANT:
		return constantInstruction("const", chunk, offset);

	case OP_NULL:
		return simpleInstruction("op_null", chunk, offset);

	case OP_TRUE:
		return simpleInstruction("op_t", chunk, offset);

	case OP_FALSE:	
		return simpleInstruction("op_f", chunk, offset);

	case OP_POP:
		return simpleInstruction("pop", offset);

	case OP_GET_LOCAL:
		return byteInstruction("local_g", chunk, offset);

	case OP_SET_LOCAL:
		return byteInstruction("local_s", chunk, offset);

	case OP_DEFINE_GLOBAL:
		return constantInstruction("global_d", chunk, offset);

	case OP_GET_GLOBAL:
		return constantInstruction("global_g", chunk, offset);

	case OP_SET_GLOBAL:
		return constantInstruction("global_s", chunk, offset);

	case OP_EQUAL:
		return simpleInstruction("op_eq", chunk, offset);

	case OP_GREATER:
		return simpleInstruction("op_g", chunk, offset);

	case OP_LESS:
		return simpleInstruction("op_l", chunk, offset);

	case OP_NEGATE:
		return simpleInstruction("neg", offset);

	case OP_ADD:
		return simpleInstruction("add", offset);

	case OP_SUBTRACT:
		return simpleInstruction("sub", offset);

	case OP_MULTIPLY:
		return simpleInstruction("mul", offset);

	case OP_DIVIDE:
		return simpleInstruction("div", offset);

	case OP_NOT:
		return simpleInstruction("not", offset);

	case OP_PRINT:
		return simpleInstruction("print", offset);

	case OP_JUMP:
		return jumpInstruction("jump", 1, chunk, offset);

	case OP_LOOP:
		return jumpInstruction("loop", -1, chunk, offset);

	case OP_JUMP_IF_FALSE:
		return jumpInstruction("bnf", 1, chunk, offset);

	case OP_RETURN:
		return simpleInstruction("return", offset);

	default:
		printf("Unknown opcode %d\n", instruction);
		return offset + 1;
	}

	return 0;
}
