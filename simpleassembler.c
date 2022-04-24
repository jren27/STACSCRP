// This is a prototype of the assembler
// It assumes that everything is formatted in a very specific way
#include <stdio.h>
#include <stdlib.h> // do I need this?
#include <string.h>
#include <strings.h> // strcasecmp
#include <stdbool.h>
#include <ctype.h>
#include "src/instruction.h"

void writeB(char c, char *b, FILE* out) {
	*b = c;
	fwrite(b, 1, 1, out);
}


void readToWhitespace(char* buff, int n, FILE* file) {
	int copyIndex = 0;
	bool beginCopy = false;
	int temp = 0;
	while (copyIndex < n) {
		temp = fgetc(file);
		if (temp == EOF) {
			return;
		}
		// This could be more efficient lol
		if (temp == ' ' || temp == '\t' || temp == '\n') {
			if (beginCopy) {
				buff[copyIndex] = '\0';
				printf("%s ", buff); // TODO DELETE THIS WHEN FULL ASSEMBLER COMPLETE
				return;
			}
		} else {
			if (!beginCopy) {
				beginCopy = true;
			}
			buff[copyIndex] = temp;
			copyIndex++;
		}
	}
	buff[copyIndex] = '\0';
}

void parseFile(FILE* in, FILE* out) {
	char buffer[50] = "";
	unsigned short toWrite;
	int intarg;
	double doublearg;
	while (!feof(in)) {

		printf("\n------\n");
		// Read stack
		readToWhitespace(buffer, 50, in);
		fputc(buffer[0] - '0', out);
		// Read instruction
		readToWhitespace(buffer, 50, in);
		if (!strcasecmp(buffer, "HALT")) {
			fputc(HALT_OP, out);
		// --- STACK OPERATIONS ---
		} else if (!strcasecmp(buffer, "POP")) {
			fputc(STAC_OP | POP_ST, out);
		} else if (!strcasecmp(buffer, "PUSH")) {
			fputc(STAC_OP | PUSH_ST, out);
		} else if (!strcasecmp(buffer, "SWAP")) {
			fputc(STAC_OP | SWAP_ST, out);
		} else if (!strcasecmp(buffer, "GRAB")) {
			fputc(STAC_OP | GRAB_ST, out);
		} else if (!strcasecmp(buffer, "DROP")) {
			fputc(STAC_OP | DROP_ST, out);
		} else if (!strcasecmp(buffer, "NEW")) {
			readToWhitespace(buffer, 50, in);
			if (!strcasecmp(buffer, "*")) {
				fputc(NEW_OP | NULL_TP, out);
			} else if (!strcasecmp(buffer, "#")) {
				fputc(NEW_OP | INT_TP, out);
				fscanf(in, "%d", &intarg);
				fwrite(&intarg, sizeof(int), 1, out);	
			} else if (!strcasecmp(buffer, "\'")) {
				fputc(NEW_OP | CHAR_TP, out);
				fscanf(in, "%c", &intarg);
				fwrite(&intarg, sizeof(char), 1, out);
			} else if (!strcasecmp(buffer, ".")) {
				fputc(NEW_OP | DOUB_TP, out);
				fscanf(in, "%lf", &doublearg);
				fwrite(&doublearg, sizeof(double), 1, out);
			}
		// --- ARITHMETIC OPERATIONS ---
		} else if (!strcasecmp(buffer, "ADD")) {
			fputc(ARTH_OP | ADD_AL, out);
		} else if (!strcasecmp(buffer, "SUB")) {
			fputc(ARTH_OP | SUB_AL, out);
		} else if (!strcasecmp(buffer, "MUL")) {
			fputc(ARTH_OP | MUL_AL, out);
		} else if (!strcasecmp(buffer, "DIV")) {
			fputc(ARTH_OP | DIV_AL, out);
		} else if (!strcasecmp(buffer, "AND")) {
			fputc(ARTH_OP | AND_AL, out);
		} else if (!strcasecmp(buffer, "LOR")) {
			fputc(ARTH_OP | LOR_AL, out);	
		} else if (!strcasecmp(buffer, "NOT")) {
			fputc(ARTH_OP | NOT_AL, out);
		} else if (!strcasecmp(buffer, "XOR")) {
			fputc(ARTH_OP | XOR_AL, out);
		// --- COMPARATOR OPERATIONS ---
		} else if (!strcasecmp(buffer, "CMEQ")) {
			fputc(COMP_OP | EQ, out);
		} else if (!strcasecmp(buffer, "CMNE")) {
			fputc(COMP_OP | NE, out);
		} else if (!strcasecmp(buffer, "CMGT")) {
			fputc(COMP_OP | GT, out);
		} else if (!strcasecmp(buffer, "CMLE")) {
			fputc(COMP_OP | LE, out);
		} else if (!strcasecmp(buffer, "CMLT")) {
			fputc(COMP_OP | LT, out);
		} else if (!strcasecmp(buffer, "CMGE")) {
			fputc(COMP_OP | GE, out);	
		} else if (!strcasecmp(buffer, "LTRL")) {
			fputc(LTRL_OP, out);
		} else if (!strcasecmp(buffer, "TYPE")) {
			fputc(TYPE_OP, out);
		} else if (!strcasecmp(buffer, "RTYP")) {
			fputc(RTYP_OP, out);
		} else if (!strcasecmp(buffer, "IPUT")) {
			fputc(IPUT_OP, out);
		} else if (!strcasecmp(buffer, "OPUT")) {
			fputc(OPUT_OP, out);	
		} else if (!strcasecmp(buffer, "GOTO")) {
			fputc(GOTO_OP, out);
		} else if (!strcasecmp(buffer, "GOIF")) {
			fputc(GOTO_OP | COND_BR, out);
		} else if (!strcasecmp(buffer, "JUMP")) {
			fputc(JUMP_OP, out);
		} else if (!strcasecmp(buffer, "JPIF")) {
			fputc(JUMP_OP | COND_BR, out);
		}
	}
	fputc(-1, out);
}

int main() { // TODO use command line arguments
	FILE* in = fopen("test.stacscrp", "r");
	FILE* out = fopen("out.bin", "w+");
	parseFile(in, out);
	fclose(in);
	fclose(out);
	return 0;
}
