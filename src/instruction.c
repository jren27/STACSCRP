#include "instruction.h"
#include <string.h>
#include <stdlib.h>

// Push to the back
void pushLine(linelist* list, char* buffer) {
	line* newline = malloc(sizeof(line));
	strncpy(newline->buff, buffer, 80);
	newline->next = NULL;
	if (list->head == NULL) {
		newline->prev = NULL;
		list->head = newline;
		list->tail = list->head;
		return;
	}
	newline->prev = list->tail;
	list->tail->next = newline;
	list->tail = list->tail->next;
}
