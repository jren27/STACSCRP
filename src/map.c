#include "map.h"

// No balancing >:D
treenode* insertNode(treenode* root, char* key, char* value) {
	if (root == NULL) {
		root = malloc(sizeof(treenode));
		strncpy(root->key, key, 50);
		strncpy(root->value, value, 50);
		root->left = root->right = NULL;
		return root;
	}	
	treenode* prevnode = NULL;
	treenode* currnode = root;
	int compare;
	while (currnode != NULL) {
		prevnode = currnode;
		compare = strcmp(currnode->key, key);
		if (compare == 0) {
			strncpy(currnode->value, value, 50);
			return root;
		}
		currnode = (compare < 0) ? currnode->left : currnode->right;
	}
	// Add another node
	compare = strcmp(prevnode->key, key);
	currnode = malloc(sizeof(treenode));
	strncpy(currnode->key, key, 50);
	strncpy(currnode->value, value, 50);
	currnode->left = currnode->right = NULL;
	if (compare < 0) {
		prevnode->left = currnode;
	}
	prevnode->right = currnode;
	return root;
}

int getNode(treenode* root, char* key, char* buffer) {
	treenode* currnode = root;
	int compare;
	while (currnode != NULL) {
		compare = strcmp(currnode->key, key);
		if (compare == 0) {
			strncpy(buffer, currnode->value, 50);
			return 0;
		}
		currnode = (compare < 0) ? currnode->left : currnode->right;
	}
	return 1;
}

void freeMap(treenode* root) {
	if (root == NULL) {
		return;
	}
	freeMap(root->left);
	freeMap(root->right);
	free(root);
}
