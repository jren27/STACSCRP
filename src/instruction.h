#ifndef INSTRUCTION
#define INSTRUCTION

#include "diskstack.h"

#define POP_OP  0x00
#define TOP_OP  0x01
#define PUSH_OP 0x02
#define SWAP_OP 0x03
#define DROP_OP 0x04
#define ARTH_OP 0x05
#define COMP_OP 0x06
#define LTRL_OP 0x07
#define TYPE_OP 0x08
#define RTYP_OP 0x09
#define IPUT_OP 0x0A
#define OPUT_OP 0x0B
#define GOTO_OP 0x0C
#define JUMP_OP 0x0D
#define EXEC_OP 0xFF

#define NULL_TP 0x00
#define  INT_TP 0x10
#define CHAR_TP 0x20
#define DOUB_TP 0x30

#define ADD_AL 0x00
#define SUB_AL 0x10
#define MUL_AL 0x20
#define DIV_AL 0x30
#define AND_AL 0x40
#define LOR_AL 0x50
#define NOT_AL 0x60
#define XOR_AL 0x00

#define EQ 0x00
#define NE 0x10
#define GT 0x20
#define LE 0x30
#define LT 0x40
#define GE 0x50

#define COND_BR 0x10


typedef struct line {
	char* unparsed[4];
	unsigned int lineNum;
	struct line* next;
	struct line* prev;
} line;

typedef struct instruction {
	disk d;
	unsigned int pos;
	struct instruction* next;
	struct instruction* prev;
} instruction;

typedef struct {
	line* head;
	line* tail;
} linelist;

typedef struct {
	instruction* head;
	instruction* tail;
} instructionlist;

void pushLine(linelist* list, line* line, bool front);
void freeLine(line* l);
line* removeLine(linelist* list, line* l);
void freeLineList(linelist* list);

void pushInstruction(instructionlist* list, instruction* inst);
void removeInstruction(instructionlist* list, instruction* l);
void freeInstructionList(instructionlist* list);

#endif
