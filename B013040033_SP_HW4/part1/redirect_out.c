/*
 * redirect_out.c   :   check for >
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_out(char ** myArgv) {
	int i = 0;
  	int fd;

  	/* search forward for >
  	 * Fill in code. */
	char *file;
	for(;myArgv[i]!=NULL;i++)
	if ( strcmp(myArgv[i],">")==0 ) {	/* found ">" in vector. */
		file=myArgv[i+1];
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		/*
		O_WRONLY : write only; O_CREAT : if nonexist->creat one; 
		O_TRUNC : empty file; 0666 : -rw-rw-rw-
		*/
		dup2(fd, STDOUT_FILENO);
		close(fd);
		
		for(;myArgv[i]!=NULL;i++)		
		{
			free(myArgv[i]);
			myArgv[i]=NULL;
		}
		execvp(myArgv[0], myArgv);
	    	/* 1) Open file.
	    	 * 2) Redirect to use it for output.
	    	 * 3) Cleanup / close unneeded file descriptors.
	    	 * 4) Remove the ">" and the filename from myArgv.
			 *
	    	 * Fill in code. */
  	}
  	return 0;
}
