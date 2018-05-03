#ifndef STDIO_H
	#include <stdio.h>
#endif

struct Node *root, *current;




void AddData(struct Node *node);
int LeafsNumber(struct Node *node);
void PrintNode(struct Node *node);
void AddChild(struct Node *node,char*, uint8_t ); // add n children to the node



int WhichChild(struct Node *node);
void PrintTree(struct Node *node );
struct Node *LevelUp(struct Node *goal, struct Node *node);
void GrowLeaf(struct Node *node);
void WriteInFile(FILE *file,struct Node* node);
void SaveTree();
struct Node* ReadOutOfFile(struct Node *node,FILE * f);
struct Node* LoadTree (char *fname);

void *TerminateTree (struct Node *node) ; // delete tree, full or partial
void InitTree ();		 // init tree
struct Node* create_node(char*);	 // create single node with empty fields
void node_from_number(int ); // set current node to the selection from GTK
void adding_node_data_row(struct Node* node); // adding row data to the node

 

