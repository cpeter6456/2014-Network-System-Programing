/*
 * lookup2 : local file ; setup an in-memory index of words
 *             and pointers into the file. resource is file name
 *             use qsort & bsearch
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "dict.h"



#include <sys/stat.h>
#include <sys/types.h>
typedef struct {
    char word[WORD];  /* The word to be looked up */
    long off;         /* Offset into file for word definition */
} Index;

/*
 * This ugly little function can be used by qsort & bsearch
 * It compares the word part of two structures of type Index
 */

int dict_cmp(const void *a,const void *b) {
	return strcmp(((Index *)a)->word,((Index *)b)->word);
}

int lookup(Dictrec * sought, const char * resource) {
	static Index * table;
	Dictrec dr;
	static int numrec;
	int i;
	Index * found,tmp;
	static FILE * in;
	static int first_time = 1;

	if (first_time) { /* set up index */
		first_time = 0;

		/* Open file.
		 *
		 * Fill in code. */
		in = fopen(resource, "rb+");
		if (!in) {
   			fclose(in);
			perror("lookup2:fopen");
			exit(1);
		}

		/* Get number records in file by dividing ending file offset by recsize.
		 *
		 * Fill in code. */

		/**b: wanted fopen file && and get file stat**/
		struct stat info;
		if(stat(resource,&info)==-1){
			perror(resource);
			exit(1);
		}
		numrec = info.st_size/512;  /**all byte**/
		

		/* Go to the beginning of the file.
		 *
		 * Fill in code. */
		rewind(in);

		/* Allocate zeroed-out memory: numrec elements of struct Index. */
		table = calloc(sizeof(Index),numrec);

		/* Read the file into the just-allocated array in memory.
		 *
		 * Fill in code. */
		int table_num=0;
		
		while(!feof(in)) {
		
			if( fread(&dr, sizeof(dr), 1, in) <0 ){
				perror("lookup2:fread1");exit(1);	
			}
			strcpy(table[table_num].word,dr.word);
			table[table_num].off=table_num;

			table_num++;
		}




		/* Sort the table of entry/offset Index structures. */
		qsort(table,numrec,sizeof(Index),dict_cmp);


		fclose(in);
	} /* end of first-time initialization */

	/* use bsearch to find word in the table; seek & read from file if found. */
	strcpy(tmp.word,sought->word);
	found = bsearch(&tmp,table,numrec,sizeof(Index),dict_cmp);

	/* If found, go to that place in the file, and read the record into the
	 * caller-supplied space. */
	if (found) {
		/* Fill in code. */
		in = fopen(resource, "rb+");
		if (!in) {
   			fclose(in);
			perror("lookup2:fopen");
			exit(1);
		}
/**fseek() 可設定在檔案中存取的位置，共需三個參數，第一個參數為指向結構 FILE 的指標，第二個參數為位移字元量，第三個參數為位移的起算位址，可以是檔案開頭 SEEK_SET 、現在位置 SEEK_CUR 或檔案結尾 SEEK_END 。**/
		fseek(in, 512*(found->off), SEEK_SET);

		if( fread(&dr, sizeof(dr), 1, in) <0 ){
			perror("lookup2:fread2");exit(1);	
		}
		printf("off = %ld text=%s\n",found->off,dr.word);
		strcpy(sought->text,dr.text);

		fclose(in);		

		return FOUND;
	}
	return NOTFOUND;
}
