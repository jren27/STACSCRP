#ifndef INSTRUCTION
#define INSTRUCTION
#include "diskstack.h"

typedef enum operation {
	POP, TOP, PUSH, SWAP, DROP, GRAB,
	ADD, SUB, MUL, DIV,
	AND, LOR, NOT, XOR,
	CMEQ, CMGT, CMLT, CMGE, CMLE, CMNE,
	NEW, NEWN, WIPE,
	LTRL, TYPE, RTYP,
	IPUT, OPUT,
	MARK, JUMP, JCON,
	HALT
} operation;

operation toOperation(unsigned short o);

typedef struct instruction {
	unsigned short stack;
	operation op;
	datatype argtype;
	int argint;
	double argdouble;
} instruction;

#endif
