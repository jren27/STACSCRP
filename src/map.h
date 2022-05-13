// Bruh
#ifndef MAP
#define MAP

#include <string.h>
#include <stdlib.h>

typedef struct treenode {
	char key[50];
	char value[50];

	struct treenode* left;
	struct treenode* right;
} treenode;

// Returns root
treenode* insertNode(treenode* root, char* key, char* value);
// Returns 0 if success, 1 if failure
int getNode(treenode* root, char* key, char* buffer);
void freeMap(treenode* root);

#endif
