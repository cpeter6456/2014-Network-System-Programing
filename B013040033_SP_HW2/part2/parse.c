/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 0;
  	char * token;
  	char **newArgv;

  	/* Nothing entered. */
  	if (line == NULL || strcmp(line,"\n")==0) {
    	return NULL;
  	}

  	/* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line.
	 *
	 * Fill in code.
     */
	token = strtok (line,delim);
	if(token == NULL)
		return NULL;

  	/* Create array with room for first token.
  	 *
	 * Fill in code.
	 */
	int temp_len=strlen(token);

	newArgv = malloc(sizeof(char*));
	newArgv[count] = malloc(sizeof(char*)*(temp_len+1));


	strncpy(newArgv[count],token,temp_len);
	newArgv[count][temp_len]='\0';		
	/* because strncpy not + '\0'  */

	count++;

  	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 * 
  	 * Fill in code.
	 */


	token = strtok (NULL,delim);


	while(token != NULL)
	{
		

		temp_len=strlen(token);

		/**false to use  realloc( newArgv,sizeof(newArgv)+sizeof(char*) )
		because sizeof(newArgv) is all 8
		**/

		newArgv = realloc( newArgv,(count+1)*sizeof(char*) );
		newArgv[count] = malloc(sizeof(char*)*(temp_len+1));


		strncpy(newArgv[count],token,temp_len);
		newArgv[count][temp_len]='\0';


		token = strtok (NULL,delim);
		count++;
	}


  	/* Null terminate the array and return it.
	 *
  	 * Fill in code.
	 */

	newArgv = realloc( newArgv,(count+1)*sizeof(char*) );

	newArgv[count]=NULL;


	/** i think [number] : something 	is printf at here
	int t=0;
	while(newArgv[t]!=NULL)
	{
		printf("[%d] : %s\n",t,newArgv[t]);
		t++;
	}
	*/
  	return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

	int i = 0;
	while(oldArgv[i]!=NULL)
	{
		free(oldArgv[i]);

		i++;
	}
	
	free(oldArgv);

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 *
	 * Fill in code.
	 */
}
