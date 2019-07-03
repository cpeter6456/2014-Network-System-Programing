/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include "shell.h"
#include <sys/types.h>
#include <errno.h>

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(char ** argv);	/* "builtin" command tells whether a command is builtin or not. */
static void bi_cd(char **argv) ;		/* "cd" command. */
static void bi_echo(char **argv);		/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_hostname(char ** argv);	/* "hostname" command. */
static void bi_id(char ** argv);		/* "id" command shows user and group of this process. */
static void bi_pwd(char ** argv);		/* "pwd" command. */
static void bi_quit(char **argv);		/* quit/exit/logout/bye command. */




/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
  	char * keyword;					/* When this field is argv[0] ... */
  	void (* do_it)(char **);		/* ... this function is executed. */
} inbuilts[] = {
  	{ "builtin",    bi_builtin },   /* List of (argv[0], function) pairs. */

    /* Fill in code. */
    { "echo",       bi_echo },
    { "quit",       bi_quit },
    { "exit",       bi_quit },
    { "bye",        bi_quit },
    { "logout",     bi_quit },
    { "cd",         bi_cd },
    { "pwd",        bi_pwd },
    { "id",         bi_id },
    { "hostname",   bi_hostname },
    {  NULL,        NULL }          /* NULL terminated. */
};


static void bi_builtin(char ** argv) {
	/* Fill in code. */
char * array[]={"echo","quit","exit","bye","logout","cd", "pwd","id","hostname" };
int flag = -1;
int i=0;
for(;i<9;i++)
	if(strcmp(argv[1],array[i])==0)
	{	flag=0;
		printf("%s is a builtin feature",argv[1]);
		break;
	}
if(flag==-1)
	printf("%s is NOT a builtin feature",argv[1]);
}

static void bi_cd(char **argv) {
	/* Fill in code. */
/*
int chdir(const char * path);
函数说明：chdir()用来将当前的工作目录改变成以参数path 所指的目录.
返回值执：行成功则返回0, 失败返回-1, errno 为错误代码.
*/



	if(  chdir(argv[1]) < 0 )	
		exit(errno);
}

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

static void bi_hostname(char ** argv) {
	/* Fill in code. */
	struct utsname uts;
	
	 if( uname(&uts) == -1 ) {
		 perror("myuname.c:main:uname");
		 exit(1);
	 }
 	printf("hostname: %s\n", uts.nodename);

}

static void bi_id(char ** argv) {
 	/* Fill in code. */
	gid_t uid=getuid();
	gid_t gid=getgid();
	struct group * data =getgrent();
	char * ch = getlogin();
 	printf("UserID: %u(%s)  GroupID: %u(%s)\n", uid,ch,gid,data->gr_name);

}

static void bi_pwd(char ** argv) {
	/* Fill in code. */
char buf[80];
char *pwd;
pwd=getcwd(buf, sizeof(buf));
if(pwd==NULL)	   
{
perror("prindir:getcwd");
exit(errno);
}
else	
	printf("%s\n", buf);


}

static void bi_quit(char **argv) {
	exit(0);
}


/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; /* close coupling between is_builtin & do_builtin */

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
