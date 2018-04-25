#ifndef STDIO_H
	#include <stdio.h>
#endif
#ifndef STDLIB_H
	#include <stdlib.h>
#endif
#ifndef STDINT_H
	#include <stdint.h> // uint8_t
#endif
#ifndef STDDEF_H 
	#include <stddef.h>
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

//______________reading data unknown length from console or file into string

char* ReadString (FILE* f) {
	int chr;
	int n=0;
	size_t size=4;
	char *st=(char*)malloc(size);
	bool loop=false;
	while (!loop) {
		while (((chr=fgetc(f))!='\n') && (chr != EOF) ) {
			st[n]=chr;
			if (n+1==size)
				st=(char*)realloc(st,(size+=8)*sizeof(char));
			n++;
		}
		loop=true;
		if (n==0)
			loop=false;
	}
	if (n!=0 )
		st = (char*)realloc(st,sizeof(char)*n);
	st[n]='\0';
return st;
}

//______________________________________________________________________________

//_____________________wrapper around malloc to handle failed alocation_________________________________________

void* wr_malloc(size_t size, char* id, void *mem_free (struct Node *node), void *pointer ) { // wrapper for malloc
	void *ret = malloc(size);
	if (!ret && !size)
              ret = malloc(1);
	if (!ret) {
		fprintf(stderr, "%s: Out of memory, id: \n",id);
  		fflush(stderr);
		// save tmp file
		mem_free(pointer);
      		memset(ret, 0xA5, size);
		exit(1);
	}
      return ret;
}
//




//____________________return variable type_____________________________________

#define datatype(x) _Generic((x),\
	_Bool:	"bool",\
	char: 	"char",\
	int:	"int",\
	double: "double",\
	char *: "pointer to char"\
)


