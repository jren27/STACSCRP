#ifndef INSTRUCTION
#define INSTRUCTION

#define HALT_OP 0x00
#define  POP_OP 0x01
#define PEEK_OP 0x02
#define PUSH_OP 0x03
#define SWAP_OP 0x04
#define GRAB_OP 0x05
#define DROP_OP 0x06
#define  NEW_OP 0x07
#define ARTH_OP 0x08
#define COMP_OP 0x09
#define TYPE_OP 0x0A
#define JUMP_OP 0x0B
#define JCON_OP 0x0C

#define NULL_TP 0x00
#define  INT_TP 0x10
#define CHAR_TP 0x20
#define BOOL_TP 0x30
#define DOUB_TP 0x40

#endif
