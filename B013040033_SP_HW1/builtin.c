/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"




/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv) {
  	/* Fill in code. */
	int temp=0;
	while(argv[temp] != NULL)temp++;
	
	/**first if to avoid just "echo" to error**/	
	if(argv[1] == NULL);
	else if( strcmp( argv[1], "-n") ==0 )
	{
		int num = 2+atoi(argv[2]);
		/* if argv[2] bigger than other token_count*/
		
		if( num >=temp)
			printf("your echo Number error\n");
		else
			printf("%s\n",argv[num]);
			
	}
	else
	{	int i=1;
		while(i != temp)
		{
			printf("%s ",argv[i++]);
		}
		printf("\n");
	}
		



}
/* Fill in code. */

static void bi_exit (char **argv) {
exit(0);
}
static void bi_quit (char **argv) {
exit(0);
}
static void bi_logout (char **argv) {
exit(0);
}
static void bi_bye (char **argv) {
exit(0);
}


/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
	char * keyword;			/* When this field is argv[0] ... */
	void (* do_it)(char **);	/* ... this function is executed. */
} inbuilts[] = {

	/* inbuilts is array of cmd type*/
	
	/*this is my first time see this code ex. void (* do_it)(char **)*/
	/* Fill in code. */
	{"exit",bi_exit},	
	{"quit",bi_quit},	
	{"logout",bi_logout},
	{"bye",bi_bye},
	{ "echo", bi_echo },		/* When "echo" is typed, bi_echo() executes.  */
	{ NULL, NULL }				/* NULL terminated. */
};




/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; 		/* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
  	struct cmd *tableCommand;

  	for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    	if (strcmp(tableCommand->keyword,cmd) == 0) {
			this = tableCommand;
			return 1;
		}
  	return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
  	this->do_it(argv);
	return 0;
}
