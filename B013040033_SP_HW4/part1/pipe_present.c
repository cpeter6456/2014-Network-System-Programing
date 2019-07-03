/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
	int index = 0;
	int flag=-1;
  	/* Search through myCurrentArgv for a match on "|". */
	for(;myCurrentArgv[index]!=NULL;index++)
  		if ( strcmp(myCurrentArgv[index],"|")==0 )
		{
		flag=0;
		break;
		}

  	if(  flag==0 && (index<1 || myCurrentArgv[index+1]==NULL) ) /* At the beginning or at the end. */ {
    		return -1;

  	} else if( flag==-1 ) /* Off the end. */ {
    	return 0;

  	} else {
    	/* In the middle. */
    	return index;
  	}
}
