/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;

	if (first_time) { 
		first_time = 0;
		/* open up the file
		 *
		 * Fill in code. */
		if ((in =fopen(resource,"r")) == NULL){DIE(resource);}
	}

	/* read from top of file, looking for match
	 *
	 * Fill in code. */
	rewind(in);
	while(!feof(in)) {
		/* Fill in code. */
		fread(&dr, sizeof(dr), 1, in);
		
		if( strcmp( (*sought).word, dr.word)==0 ){
		int i=0;
		for(;i<480;i++){
			(*sought).text[i] = dr.text[i];}

		return FOUND;
		
		}
	}

	return NOTFOUND;
}
