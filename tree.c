#ifndef STDIO_H
	#include <stdio.h>
#endif
#ifndef STDLIB_H
	#include <stdlib.h>
#endif
#ifndef STDINT_H
	#include <stdint.h> // uint8_t
#endif
#ifndef STRING_H
	#include <string.h> // string
#endif
#ifndef STDBOOL_H
	#include <stdbool.h> //bool type
#endif
#ifndef STANDLIB_H
	#include "Headers/StandLib.h"
#endif
#ifndef GTK_H
	#include <gtk/gtk.h>
#endif
#ifndef TREE_H
	#include "Headers/tree.h"
#endif

	/*union dType {		//data type
		bool dbool;	//bool
		int dint;	//int
		int *dAint;	// Dynamic array of int
		double ddouble;	// double
		double *dAdouble;	// Dynamic array of double
		char* dchar;	//char
	};*/ //we will use void*  
	

	struct Info {    //data of the single leaf
		//dType* info;
		void* input;
		uint8_t* type; // 0 if it is int, float of string; other number is a number of ellements in array for any array type			
		uint8_t num;
	};

struct Node{		
	struct Info data;   	// array of data
	int rows;
	char *name;		// node name
	struct Node** pointer;  // array of pointer for children
	struct Node* parent; 	// pointer to parent
	int num; 		// number of children
} *root=NULL,*current=NULL;

static const char* signature = "ZVProject";
static const int version = 100;

//____________________init & delete___________

//Destroy tree starting from node
void *TerminateTree (struct Node *node) {
	if (node!=NULL) {

		for (register uint8_t i=node->num; i-->0;) {  	// for (int i= number_of_children; i>0; i--)
			TerminateTree(node->pointer[i]); 	//recursia
		}

		free(node->name); 				//free the name, allocated in strdup
		free(node); 					//I'm freeeees
	}
}

//Reindex elements of parent array of children after removing on of them 
void Reindex(struct Node *node, int index) {	
	memmove(node->pointer+index, node->pointer+index+1,(node->num-1-index)*sizeof(struct Node*));
	node->pointer[node->num--]=NULL;
}

//warpper around deletion and reindex
void TerminateAndReindex (struct Node *node ) {
	struct Node *parent=node->parent; 		//get the parent 
	register uint8_t i=0;
	while (parent->pointer[i]!=node)		//get the index of deleting childre
		i++;	
	TerminateTree(node);
	if (i!=parent->num-1)				// check if deleted child is not a last in the list
		Reindex(parent,i);			// no realocation to save time 
	else 
		--parent->num;				//no realocation
	PrintTree(root);
}

//init tree 
void InitTree () {
	if ((root) || (current)) {	// check if pointers for root and current exists
		current=root;		// make current node our root
		TerminateTree(root);	// delete tree if exist
		root = NULL;		// make sure root is NULL
		current = NULL;		// make sure current is NULL
	}
	root = create_node(NULL);	// creating a root
	root->parent=NULL;
	current = root;			// assign current as root
}
//____________________________________________________

//creating EMPTY node
struct Node* create_node(char* name) {
	struct Node* node = (struct Node*)wr_malloc(sizeof(struct Node),"001",TerminateTree,root); // malloc id:1
	node->num=0;								// number of child is 0
	node->rows = 1; 							//data rows 1
	if (!name)
		name = "Root\0"; 
	node->name = strdup(name); 					// node name
	//node->data = (struct Info*)wr_malloc(sizeof(struct Info),"002",TerminateTree,root);//malloc id:2
	//node->data[node->rows-1].input=(char*)"empty\0"; 			// declare type since field is void*
	//node->data[node->rows-1].type= 1; 					// 1 for string
	return node;
}
/*
//creating another data row
void adding_node_data_row(struct Node* node) {
	node->rows++;						//one more row
	node->data = (struct Info*)realloc(node->data,(node->rows)*sizeof(struct Info*));
	node->data[node->rows-1].input=(char*)"empty\0";	// declare type since field is void*
	node->data[node->rows-1].type= 1; 			// 1 for string
}
*/

//______________________________________________________

//________________________Child form selection__________
//Select index of child
void node_from_path(int* path, int depth){  		//array of [depth] ellement
	current= root;					//set up to the begining
	for (register uint8_t k=0; ++k<depth;) {	//get depth-1 times
		current=current->pointer[path[k]];	//get index for next position from path array
	}	
}


//__________________________ADD children_____________
//Add childrens to the node

void AddChild(struct Node *node,char* name ,uint8_t n) {
	
	for(register uint8_t i=n; i-->0;){                   //loop for terminal mode
		struct Node* leaf=create_node(name); 	// creating child node
		if (node->num == 0) {		// if it first child than allocate memory for array
			node->pointer=(struct Node**)wr_malloc(sizeof(struct  Node*),"003",TerminateTree,root); // malloc id:3 
		}
		else {
			node->pointer=(struct Node**)realloc(node->pointer,(node->num+1)*sizeof(struct Node*));
		}
		node->pointer[node->num]=leaf; //assign new child
		leaf->parent = node;
		++node->num;	// increment number of children
	}
}

//Print tree starting from selected node
void PrintTree(struct Node *node ) {
	if (node!=NULL) {
		printf("m=%d ",node->num);
		//printf("%s ",node->name);
		for (int i=0;i<node->num;i++) {
			PrintTree(node->pointer[i]);
		}
	}
	printf("\n");
}


/*	//ADD into terminal console
uint8_t n=0;
	while (n<=0){
		printf("how many children you would like to add\n");
		scanf("%d",&n);
	}


int WhichChild(struct Node *node) { 
	int n=0;
	printf("Which child to select )?\n");
	for (int i=0; i++<node->num;) {
		printf("%d ",i);
	}
	printf("\n");
	while ((n<=0)||(n>node->num)) {
		scanf("%d",&n);
		if ((n<=0)||(n>node->num))
			printf("Child not exist\n");
	}	
	return --n;	
}*/


 
/*
//Add or change data of the node
void AddData(struct Node *node) {  
	printf("Write your int\n");
	int k=0;
	scanf("%d",&k);
	node->data.test=k;
}

//Number of children of the node
int LeafsNumber(struct Node *node) {	
	return node->num;
}

//Print data from selected node
void PrintNode(struct Node *node) {  
	printf("Data %d\n",node->data.test);
}





//Move up 
struct Node *LevelUp(struct Node *goal, struct Node *node) {
	for (uint8_t j=node->num; j-->0;) 
		if (node->pointer[j]==goal) 
			return node;		
	for (uint8_t i=node->num; i-->0;) {
		struct Node* tmp = LevelUp(goal,node->pointer[i]);
		if (tmp!=NULL) 
			return tmp;
	}
	return NULL;						
}	

//Add leaf to the trunk with existet children, move all existed children on one level down
void GrowLeaf(struct Node *node) {  
	struct Node* leaf = (struct Node*)malloc(sizeof(struct Node));
	int n = WhichChild(node);
	
	leaf->pointer=(struct Node**)malloc(sizeof(struct Node*));
	++leaf->num;
	leaf->pointer[0]=node->pointer[n];
	node->pointer[n]=leaf;
}



void WriteInFile(FILE *file,struct Node* node) {
	if (node!=NULL) {
		fwrite(&node->data,sizeof(struct Info),1,file);
		int n=node->num;
		fwrite(&n,sizeof(int),1,file);
		for (int i=0;i<node->num;i++) 
			WriteInFile(file,node->pointer[i]);
	}
}

void SaveTree() {
	bool loop=false;
	FILE *file;
	uint8_t n=2;
	char *fname;
	while (!loop) {
		n=2;
		printf("Name of the file and distination\n");
		fname= ReadString(stdin);	
		int len=strlen(fname);
		fname = (char*)realloc(fname,sizeof(char)*(len+4));
		strcat(fname,".zvt"); 
		printf("%s\n",fname);
		file=fopen(fname,"rb");
		if (file!=NULL) {
			while ((n!=0)&&(n!=1)) {
				printf("file exist, do you want overwrite it? 1- yes, 0 - no\n");
				scanf("%d",&n);
			}
			fclose(file);
			if (n==1) {
				
				file=fopen(fname,"wb");
				if (file==NULL) 
					printf("No brain");
				loop=true;
			}		
		}
		else {
			file=fopen(fname, "wb");
			loop=true;
		}
		free (fname);
	}
	WriteInFile(file,root);
	fclose(file);
}

struct Node* ReadOutOfFile(struct Node *node,FILE * f){
	if (node==NULL) 
		node=(struct Node*)malloc(sizeof(struct Node));
	fread(&node->data,sizeof(struct Info),1,f);
	uint8_t n=2;
	fread(&n,sizeof(int),1,f);
	for(uint8_t i=n; i-->0;){
		struct Node* leaf=(struct Node*)malloc(sizeof(struct Node));
		if (node->num == 0) {
			node->pointer=(struct Node**)malloc(sizeof(struct Node*));
			++node->num;
		}
		else{
			struct Node** tmp=(struct Node**)realloc(node->pointer,(++node->num)*sizeof(struct Node*));
			if (!tmp)
				printf("Error in memory alocation\n");
		}
		node->pointer[node->num-1]=leaf;

	}
	for (int i=0;i<node->num;i++) 
		ReadOutOfFile(node->pointer[i],f);	
	return node;
}

struct Node* LoadTree (char *fname) {
	FILE *file=fopen(fname,"rb");
	/*bool loop=false;
	FILE *file;
	while (!loop ) {
		printf("Name of the file?");
		char* fname=ReadString(stdin);
		int len=strlen(fname);
		fname = (char*)realloc(fname,sizeof(char)*(len+4));
		strcat(fname,".zvt");
		if ((file=fopen(fname,"rb"))!=NULL) 		
			loop=true;		
		else 
			printf("file not exist\n");
		free(fname);	
	}*/
/*	root = ReadOutOfFile(root,file);
	return root;
}


//-----------------------------------------

/*
struct Node* Door () {
	struct Node* node=NULL;
	uint8_t k=2;
	while ((k!=0)&&(k!=1)) {
		printf("Do you want create new tree -0 or edit existed - 1\n");
		scanf("%d",&k);	
	}
	if (k==0) {
		node = (struct Node*)malloc(sizeof(struct Node));
	}
	else {
		printf("Open file ...\n");
		node = LoadTree();
	}
	k=2; 
	return node;	
}



*/
