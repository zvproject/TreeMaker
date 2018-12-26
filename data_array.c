#ifndef STDIO_H
	#include <stdio.h>
#endif
#ifndef STDLIB_H
	#include <stdlib.h>
#endif
#ifndef DATA_ARRAY_H
	#include "data_array.h"
#endif

int actualSize(struct DataArray* a){
	register int i = a->size-1;
	while(i>=0){
		if (a->list[i]!=NULL)
			return ++i;
		--i;
	}
	return ++i;
}

//_______constructor______________
struct DataArray *initArray() {
	struct DataArray *a = (struct DataArray*)malloc(sizeof(struct DataArray)); // pointer to table
	a->size = DEFAULT_SIZE;
	a->list = (struct Element**)malloc(sizeof(struct Element*)*DEFAULT_SIZE); //array for data
	for(int i=0; i<DEFAULT_SIZE; i++)
		a->list[i] = NULL;
	return a;
}

void cleanArray(struct DataArray* a){
	for(register int i=0; i<actualSize(a);i++)
		free(a->list[i]);
	free(a->list);
	free(a);
}
//_______________________
//relocate(add(sign=1) or remove(sign=-1)) memory with scified size(0-for deafault)
struct DataArray *reallocateArray(struct DataArray* a, int size, int sign){
	if (size == 0)
		size = DEFAULT_SIZE;
	int new_size = sign*size;
	a->list = (struct Element**)realloc(a->list, sizeof(struct Element*)*(a->size+new_size));
	if (new_size > 0) {
		for(int i=a->size; i<a->size+new_size; i++)
			a->list[i] = NULL;
	}
	a->size += new_size;
	return a;
}

//create new data element
struct Element *newDataElement(void* ptr, int type){
 	struct Element *el = (struct Element*)malloc(sizeof(struct Element));
 	el->DataPointer = ptr;
 	el->type = type;
 	return el;
}

//add to data array data with it type on position index(<=0 to add at the end)
void addDataElement(struct DataArray* a, void* ptr, int type, int index){
	--index;
	int arraySize = actualSize(a);
	if ((index < 0) || (index>arraySize-1))
		index = arraySize;
	for(register int i=arraySize+1; --i>index;)
		a->list[i]=a->list[i-1];
	a->list[index] = newDataElement(ptr, type);
	if (a->size == arraySize+1)
		a = reallocateArray(a, 0, 1);
}

// changed the data of data element
void changeData(struct DataArray* a, void* ptr, int type, int index){
	a->list[index-1]->type = type;
	a->list[index-1]->DataPointer = ptr;
}

//remove the element from data array on position index(<=0 to remove the end)
void removeDataElement(struct DataArray* a, int index){
	--index;
	int arraySize = actualSize(a);
	if((index>=arraySize) || (index < 0))
		printf("Error: Wrong index/n");
	else {
		free(a->list[index]);
		a->list[index] = NULL;
		for(register int i=index; ++i<arraySize;)
			a->list[i-1] = a->list[i];
	}
	if (a->size-2*DEFAULT_SIZE+1 == arraySize-1 )
		a = reallocateArray(a, 0, -1);
}