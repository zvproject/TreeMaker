#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H

struct Element {
	void* DataPointer;
	int type;
};

struct DataArray {
	int size;
	struct Element **list;
}; 

extern const int DEFAULT_SIZE = 5;

int actualSize(struct DataArray*);
struct DataArray *initArray();
void cleanArray(struct DataArray*);
struct DataArray *reallocateArray(struct DataArray*, int, int);
struct Element *newDataElement(void*, int);
void addDataElement(struct DataArray*, void*, int, int);
void changeData(struct DataArray*, void*, int, int);
void removeDataElement(struct DataArray*, int);

#endif