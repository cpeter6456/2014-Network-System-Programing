/*
 * lookup5 : local file ; setup a memory map of the file
 *           Use bsearch. We assume that the words are already
 *           in dictionary order in a file of fixed-size records
 *           of type Dictrec
 *           The name of the file is what is passed as resource
 */

#include <stdlib.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 

#include "dict.h"

/*
 * This obscure looking function will be useful with bsearch
 * It compares a word with the word part of a Dictrec
 */

int dict_cmp(const void *a,const void *b) {
	return strcmp( (char *)a,((Dictrec *)b)->word );
}

int lookup(Dictrec * sought, const char * resource) {
	static Dictrec * table;
	static int numrec;
	Dictrec * found, temp;
	
	static int first_time = 1;

	
	if (first_time) {  /* table ends up pointing at mmap of file */
		int fd;
		long filsiz;

		first_time = 0;

		/* Open the dictionary.
		 * Fill in code. */
		
		fd = open(resource,O_RDWR|O_CREAT,0666);
		if (!fd) {
   			close(fd);
			perror("lookup5:open");
			exit(1);
		}
		/* Get record count for building the tree. */
		filsiz = lseek(fd,0L,SEEK_END);
		numrec = filsiz / sizeof(Dictrec);

		/* mmap the data.
		 * Fill in code. */
/*
		should not use shm_open,etc ; I should 
*/
		if ((table = mmap(NULL, numrec*sizeof(Dictrec),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED)
			DIE("lookup5:mmap failed");

		qsort(table,numrec,sizeof(Dictrec),dict_cmp);
		
		close(fd);
	}

	/* search table using bsearch
	 * Fill in code. */
	strcpy(temp.word,sought->word);

	found = bsearch(&temp,table,numrec,sizeof(Dictrec),dict_cmp);

	
	if (found) {
		
		strcpy(sought->text,found->text);
		return FOUND;
	}

	return NOTFOUND;
}