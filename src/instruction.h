#ifndef INSTRUCTION
#define INSTRUCTION

#define HALT_OP 0x00
#define STAC_OP 0x01
#define  NEW_OP 0x02
#define ARTH_OP 0x03
#define COMP_OP 0x04
#define LTRL_OP 0x05
#define TYPE_OP 0x06
#define RTYP_OP 0x07
#define IPUT_OP 0x08
#define OPUT_OP 0x09
#define GOTO_OP 0x0A
#define JUMP_OP 0x0B

#define  POP_ST 0x00
#define PEEK_ST 0x10
#define PUSH_ST 0x20
#define SWAP_ST 0x30
#define GRAB_ST 0x40
#define DROP_ST 0x50

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

#define COND_BR 0x01

#endif
