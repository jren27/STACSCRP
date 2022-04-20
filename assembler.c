#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// What it says on the tin
// Works with spaces and tabs (even though I use tabs, this converts to spaces lol)
void removeWhitespace(char* line) { // What if first line is space?
	int size = 0;
	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i])
		size++;
	}
	char* newline = malloc(sizeof(char) * size)
	int i, x;
	for (i = x = 0; line[i] != '\0'; i++) {
		if (line[i] != ' ' || (i > 0 && line[i-1] != ' ')) {
			line[x++] = line[i];
		}
	}
	line[x] = '\0';
}

void strip_extra_spaces(char* str) {
	int i, x;
	for(i=x=0; str[i]; ++i)
		if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1])))
			str[x++] = str[i];
	str[x] = '\0';
}

// --- FIRST PASS ---
// The first pass will:
// - Check syntax/valid args
// - Store any DEFN lines
// - Remove whitespace/newline
int firstStack() {

	return 0;
}

// --- SECOND PASS ---
// The second pass will:
// - Convert assembly to machine codes (a bunch of ints)
int main(int argc, char* argv[]) {
	char* testline = "this    isa      test!   ";
	strip_extra_spaces(testline);
	printf("%s", testline);
	return 0;
}
